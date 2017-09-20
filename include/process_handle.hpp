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

#if defined(_WIN32)

    #include "process_handle/windows_storage.hpp"

#elif defined(__linux__)
    #include "process_handle/linux_storage.hpp"
#elif defined(__APPLE__)
    #include "process_handle/osx_storage.hpp"
#else
    #error unknown operating system
#endif // defined(_WIN32)

namespace jm {

    using pid_t           = detail::pid_t;
    using native_handle_t = detail::native_handle_t;

    class process_handle {
        detail::handle_storage _storage;

    public:
        /// \brief Default constructed process_handle is treated as our own process
        explicit process_handle() noexcept(noexcept(detail::handle_storage())) = default;

        ~process_handle() noexcept = default;

        /// \brief Opens a handle to process of given id
        explicit process_handle(pid_t process_id) noexcept(noexcept(detail::handle_storage(process_id)))
                : _storage(process_id) {}

        /// \brief Opens a handle to process of given id, sets an error code on failure
        explicit process_handle(pid_t process_id
                                , std::error_code& ec) noexcept(noexcept(detail::handle_storage(process_id, ec)))
                : _storage(process_id, ec) {}

#ifndef __linux__ // on linux handle == pid

        /// \brief Adopts an existing handle
        explicit process_handle(native_handle_t handle) noexcept(noexcept(detail::handle_storage(handle)))
                : _storage(handle) {}

#endif

        /// \brief Copy constructible.
        process_handle(const process_handle& other) noexcept = default;
        /// \brief Copy assignable.
        process_handle& operator=(const process_handle& other) noexcept = default;

        /// \brief Move constructible
        process_handle(process_handle&& other) noexcept = default;
        /// \brief Move assignable.
        process_handle& operator=(process_handle&& other) noexcept = default;

        // \brief Assigns a new native handle
        process_handle& operator=(native_handle_t handle) noexcept(noexcept(_storage.operator=(handle)))
        {
            _storage = handle;
            return *this;
        }

        /// \brief Returns the native platform specific handle.
        native_handle_t native() const noexcept { return _storage.native(); }

        /// \brief Invalidates handle stored in current object
        /// \note On windows handles are reference counted and may not be closed upon call to this function.
        void reset(std::nullptr_t p = nullptr) noexcept { _storage.reset(); }

        /// \brief Sets the stored handle to given handle and then closes the old handle.
        /// \param native_handle The new handle to replace old one with.
        /// \note On windows handles are reference counted and may not be closed upon call to this function.
        void reset(native_handle_t native_handle) noexcept { _storage.reset(native_handle); }

        /// \brief The id of process that we have the handle to.
        pid_t owner_id() const noexcept(noexcept(_storage.pid)) { return _storage.pid(); }

        /// \brief The id of process that we have the handle to.
        pid_t owner_id(std::error_code& ec) const noexcept { return _storage.pid(ec); }

        /// \brief Check whether the handle is valid.
        explicit operator bool() const noexcept { return _storage.valid(); }
    };

} // namespace

#endif // include guard
