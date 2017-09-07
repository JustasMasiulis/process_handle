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

#ifndef JM_PROCESS_HANDLE_HPP
#define JM_PROCESS_HANDLE_HPP

#include "detail/config.hpp"

#if defined(JM_PROCESS_HANDLE_WINDOWS)
    #include "detail/handle_storage_windows.hpp"
#elif defined(JM_PROCESS_HANDLE_LINUX)
    #include "detail/handle_storage_linux.hpp"
#elif defined(JM_PROCESS_HANDLE_APPLE)
    #include "detail/handle_storage_apple.hpp"
#endif

namespace jm {

    using pid_t           = detail::pid_t;
    using native_handle_t = detail::native_handle_t;

    class process_handle
    {
        detail::handle_storage _storage;

    protected:

        ~process_handle() noexcept {}

    public:
        /// \brief Default constructed process_handle is treated as our own process
        explicit process_handle() = default;

        /// \brief Opens a handle to process of given id
        explicit process_handle(pid_t process_id)
            : _storage(process_id) {}

        /// \brief Opens a handle to process of given id, sets an error code on failure
        explicit process_handle(pid_t process_id, std::error_code& ec)
            : _storage(process_id, ec) {}

#ifndef JM_PROCESS_HANDLE_LINUX // on linux handle == pid
        /// \brief Adopts an existing handle
        explicit process_handle(native_handle_t handle)
            : _storage(handle) {}
#endif

        process_handle(const process_handle& other) noexcept
            : _storage(other._storage) {}

        process_handle& operator= (const process_handle& other) noexcept
        {
            _storage = other._storage;
            return *this;
        }

        process_handle(process_handle&& other) noexcept
            : _storage(std::move(other._storage)) {}

        process_handle& operator= (process_handle&& other) noexcept
        {
            _storage = std::move(other._storage);
            return *this;
        }

        process_handle& operator= (native_handle_t handle)
        {
            _storage = handle;
            return *this;
        }


        native_handle_t native() const noexcept { return _storage.native(); }

        void reset() noexcept { _storage.invalidate(); }

        pid_t owner_id() const noexcept { return _storage.pid(); }

        operator const native_handle_t&() const noexcept { return _storage.native(); }

        explicit operator bool() const noexcept { return _storage.valid(); }
    };

} // namespace

#endif // include guard
