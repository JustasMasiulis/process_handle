#ifndef JM_PROCESS_HANDLE_STORAGE_APPLE_HPP
#define JM_PROCESS_HANDLE_STORAGE_APPLE_HPP

#include <system_error>
#include <mach/mach.h>
#include <sys/types.h>
#include <unistd.h>

namespace jm { namespace detail {

    using pid_t           = ::pid_t;
    using native_handle_t = ::mach_port_t;


    class handle_storage
    {
        ::mach_port_t _handle;

    public:
        explicit handle_storage()
            : _handle(::mach_task_self())
        {}

        explicit handle_storage(native_handle_t handle)
            : _handle(handle) {}

        explicit handle_storage(pid_t pid)
        {
            const auto kr = task_for_pid(mach_task_self(), pid, &_handle);
            if (kr != KERN_SUCCESS) {
                // Not sure what it is - was integral but documented as pointer
                _handle = (::mach_port_t)(0); 
                throw std::system_error(std::error_code(kr, std::system_category())
                                        , "task_for_pid() failed");
            }
        }

        explicit handle_storage(pid_t pid, std::error_code& ec)
        {
            const auto kr = task_for_pid(mach_task_self(), pid, &_handle);
            if (kr != KERN_SUCCESS) {
                // Not sure what it is - was integral but documented as pointer
                _handle = (::mach_port_t)(0);
                ec = std::error_code(kr, std::system_category());
            }
        }

        handle_storage(const handle_storage& other) noexcept
            : _handle(other._handle) {}

        handle_storage& operator= (const handle_storage& other) noexcept
        {
            _handle = other._handle;
            return *this;
        }

        handle_storage(handle_storage&& other) noexcept
            : _handle(other._handle) {}

        handle_storage& operator= (handle_storage&& other) noexcept
        {
            _handle = other._handle;
            return *this;
        }

        handle_storage& operator= (native_handle_t handle)
        {
            _handle = handle;
            return *this;
        }


        bool valid() const noexcept { return _handle == (::mach_port_t)(0); }

        void invalidate() noexcept { _handle = (::mach_port_t)(0); }

        const native_handle_t& native() const noexcept { return _handle; }

        pid_t pid() const noexcept { return _handle; }
    }; // handle_storage

} // namespace

#endif // include guard
