#ifndef JM_PROCESS_HANDLE_STORAGE_LINUX_HPP
#define JM_PROCESS_HANDLE_STORAGE_LINUX_HPP

#include <system_error>
#include <sys/types.h>
#include <unistd.h>

namespace jm { namespace detail {

    using pid_t           = ::pid_t;
    using native_handle_t = pid_t;


    class handle_storage
    {
        native_handle_t _handle;

    public:
        explicit handle_storage() 
            : _handle(::getpid()) {}

        explicit handle_storage(native_handle_t handle)
            : _handle(handle) {}

        explicit handle_storage(int pid)
            : _handle(pid) {}

        handle_storage(const handle_storage& other) noexcept
            : _handle(other._handle) {}

        handle_storage& operator= (const handle_storage& other) noexcept
        {
            _handle = other._handle;
            return *this;
        }

        handle_storage(handle_storage&& other) noexcept
            : _handle(other._handle) 
        {
            other.invalidate();
        }

        handle_storage& operator= (handle_storage&& other) noexcept
        {
            _handle = other._handle;
            other.invalidate();
            return *this;
        }

        handle_storage& operator= (native_handle_t handle)
        {
            _handle = handle;
            return *this;
        }


        bool valid() const noexcept { return _handle != -1; }

        void invalidate() noexcept { _handle = -1; }

        const native_handle_t& native() const noexcept { return _handle; }

        int pid() const noexcept { return _handle; }
    }; // handle_storage

} // namespace

#endif // include guard
