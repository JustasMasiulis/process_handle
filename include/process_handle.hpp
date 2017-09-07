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
        void invalidate_handle() noexcept { _storage.invalidate(); }

        bool handle_valid() const noexcept { return _storage.valid(); }

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

        pid_t pid() const noexcept { return _storage.pid(); }

        operator const native_handle_t&() const noexcept
        {
            return _storage.native();
        }
    };

} // namespace

#endif // include guard
