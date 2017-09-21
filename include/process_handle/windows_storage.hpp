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

    extern "C" {
    __declspec(dllimport) int __stdcall CloseHandle(void* handle);
    __declspec(dllimport) void*
    __stdcall OpenProcess(unsigned long desired_access, int inherit_handle, unsigned long process_id);
    __declspec(dllimport) unsigned long __stdcall GetCurrentProcessId();
    __declspec(dllimport) unsigned long __stdcall GetProcessId(void* handle);
    }

    constexpr unsigned long SYNCHRONIZE_              = 0x00100000;
    constexpr unsigned long STANDARD_RIGHTS_REQUIRED_ = 0x000F0000;
    constexpr unsigned long PROCESS_ALL_ACCESS_       = (STANDARD_RIGHTS_REQUIRED_ | SYNCHRONIZE_ | 0xFFF);

    using pid_t           = int;
    using native_handle_t = void*;

    class handle_storage {
        std::shared_ptr<void> _handle;

        struct handle_deleter_t {
            inline void operator()(native_handle_t handle) noexcept
            {
                if (handle)
                    CloseHandle(handle);
            }
        };

    public:
        explicit handle_storage()
                : handle_storage(static_cast<int>(GetCurrentProcessId())) {}

        explicit handle_storage(native_handle_t handle)
                : _handle(handle, handle_deleter_t{}) {}

        explicit handle_storage(pid_t pid)
                : handle_storage(OpenProcess(PROCESS_ALL_ACCESS_, 0, static_cast<unsigned long>(pid)))
        {
            if (_handle.get() == nullptr)
                throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category())
                                        , "OpenProcess() failed");
        }

        explicit handle_storage(pid_t pid, std::error_code& ec)
                : handle_storage(OpenProcess(PROCESS_ALL_ACCESS_, 0, static_cast<unsigned long>(pid)))
        {
            if (_handle.get() == nullptr)
                ec = std::error_code(static_cast<int>(GetLastError()), std::system_category());
        }

        handle_storage(const handle_storage& other) noexcept = default;
        handle_storage& operator=(const handle_storage& other) noexcept = default;

        handle_storage(handle_storage&& other) noexcept = default;
        handle_storage& operator=(handle_storage&& other) noexcept = default;

        bool valid() const noexcept { return static_cast<bool>(_handle); }

        void reset() noexcept { _handle.reset(); }

        void reset(native_handle_t new_handle) noexcept { _handle.reset(new_handle, handle_deleter_t{}); }

        native_handle_t native() const noexcept { return _handle.get(); }

        pid_t pid() const noexcept { return static_cast<int>(GetProcessId(_handle.get())); }

        inline pid_t pid(std::error_code&) const noexcept { return pid(); }
    }; // handle_storage

}} // namespace jm::detail

#endif // include guard
