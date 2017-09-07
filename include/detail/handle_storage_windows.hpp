/*
* Copyright 2017 Justas Masiulis
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef JM_PROCESS_HANDLE_STORAGE_WINDOWS_HPP
#define JM_PROCESS_HANDLE_STORAGE_WINDOWS_HPP

#include <memory>
#include <system_error>

namespace jm { namespace detail {

#if defined(JM_PROCESS_HANDLE_CUSTOM_WINDOWS_INCLUDE)
    JM_PROCESS_HANDLE_CUSTOM_WINDOWS_INCLUDE;
#else
    #define NOMINMAX
    #include <Windows.h>
    #undef NOMINMAX 
#endif

    using pid_t           = int;
    using native_handle_t = HANDLE;

    class handle_storage
    {
        using rp_handle = typename std::remove_pointer<HANDLE>::type;
        std::shared_ptr<rp_handle> _handle;

        static void handle_deleter(native_handle_t handle) noexcept
        {
            if (handle)
                CloseHandle(handle);
        }

    public:
        explicit handle_storage() 
            : handle_storage(static_cast<int>(GetCurrentProcessId()))
        {}

        explicit handle_storage(native_handle_t handle)
            : _handle(handle, handle_deleter)
        {}

        explicit handle_storage(pid_t pid)
            : _handle(OpenProcess(PROCESS_ALL_ACCESS, 0, static_cast<unsigned long>(pid)))
        {
            if (_handle.get() == nullptr)
                throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "OpenProcess() failed");
        }

        explicit handle_storage(pid_t pid, std::error_code& ec)
            : _handle(OpenProcess(PROCESS_ALL_ACCESS, 0, static_cast<DWORD>(pid)))
        {
            if (_handle.get() == nullptr)
                ec = std::error_code(static_cast<int>(GetLastError()), std::system_category());
        }

        handle_storage(const handle_storage& other) noexcept
            : _handle(other._handle) {}

        handle_storage& operator= (const handle_storage& other) noexcept
        {
            _handle = other._handle;
            return *this;
        }

        handle_storage(handle_storage&& other) noexcept
            : _handle(std::move(other._handle)) {}

        handle_storage& operator= (handle_storage&& other) noexcept
        {
            _handle = std::move(other._handle);
            return *this;
        }

        handle_storage& operator= (native_handle_t handle)
        {
            _handle.reset(handle);
            return *this;
        }


        bool valid() const noexcept { return _handle.get() != nullptr; }

        void invalidate() noexcept { _handle.reset(); }

        const native_handle_t& native() const noexcept { return _handle.get(); }

        pid_t pid() const noexcept { return static_cast<int>(GetProcessId(_handle.get())); }
    }; // handle_storage

}} // namespace jm::detail

#endif // include guard
