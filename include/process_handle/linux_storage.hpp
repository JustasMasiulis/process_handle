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

#ifndef JM_PROCESS_HANDLE_STORAGE_LINUX_HPP
#define JM_PROCESS_HANDLE_STORAGE_LINUX_HPP

#include <system_error>
#include <sys/types.h>
#include <unistd.h>

namespace jm { namespace detail {

    using pid_t           = ::pid_t;
    using native_handle_t = pid_t;

    class handle_storage {
        native_handle_t _handle;

    public:
        explicit handle_storage() noexcept
                : _handle(::getpid()) {}

        explicit handle_storage(native_handle_t handle, std::error_code&) noexcept
                : _handle(handle) {}

        explicit handle_storage(native_handle_t handle) noexcept
                : _handle(handle) {}

        handle_storage(const handle_storage& other) noexcept = default;
        handle_storage& operator=(const handle_storage& other) noexcept = default;

        handle_storage(handle_storage&& other) noexcept
                : _handle(other._handle)
        {
            other.reset();
        }

        handle_storage& operator=(handle_storage&& other) noexcept
        {
            _handle = other._handle;
            other.reset();
            return *this;
        }

        bool valid() const noexcept { return _handle != -1; }

        void reset() noexcept { _handle = -1; }

        void reset(native_handle_t new_handle) noexcept { _handle = new_handle; }

        native_handle_t native() const noexcept { return _handle; }

        pid_t pid() const noexcept { return _handle; }

        inline pid_t pid(std::error_code&) const noexcept { return _handle; }
    }; // handle_storage

}} // namespace jm::detail

#endif // include guard
