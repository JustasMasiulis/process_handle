#ifndef JM_PROCESS_HANDLE_HPP
#define JM_PROCESS_HANDLE_HPP

#include "detail/config.hpp"
#if defined(JM_PROCESS_HANDLE_WINDOWS)
    #include "detail/handle_storage_windows.hpp"
#elif defined(JM_PROCESS_HANDLE_LINUX)
#elif defined(JM_PROCESS_HANDLE_APPLE)
#endif

namespace jm {

    using native_handle_t = detail::native_handle_t;

    class process_handle
    {
        detail::handle_storage _storage;

    protected:
        void invalidate_handle() noexcept { _storage.invalidate(); }

        ~process_handle() noexcept {}

    public:
        /// \brief Default constructed process_handle is treated as our own process
        explicit process_handle() = default;

        /// \brief Opens a handle to process of given id
        explicit process_handle(int process_id)
            : _storage(process_id) 
        {}

        /// \brief Opens a handle to process of given id
        explicit process_handle(int process_id, std::error_code& ec)
            : _storage(process_id, ec)
        {}

        /// \brief Adopts an existing handle
        process_handle(native_handle_t handle)
            : _storage(handle)
        {}

        process_handle(const process_handle&);
        process_handle& operator= (const process_handle&);

        process_handle(process_handle&&);
        process_handle& operator= (process_handle&&);

        process_handle& operator= (native_handle_t handle)
        {
            _storage = handle;
            return *this;
        }

        int pid() const noexcept { return _storage.pid(); }

        operator const native_handle_t&() const noexcept
        {
            return _storage.native();
        }
    };

} // namespace

#endif // include guard
