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

    using native_handle_t = HANDLE;

    class handle_storage
    {
        using rp_handle = typename std::remove_pointer<HANDLE>::type;
        std::shared_ptr<rp_handle> _handle;

        static void handle_deleter(HANDLE handle) noexcept
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

        explicit handle_storage(int pid)
            : _handle(OpenProcess(PROCESS_ALL_ACCESS, 0, static_cast<unsigned long>(pid)))
        {
            if (_handle.get() == nullptr)
                throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "OpenProcess() failed");
        }

        explicit handle_storage(int pid, std::error_code& ec)
            : _handle(OpenProcess(PROCESS_ALL_ACCESS, 0, static_cast<unsigned long>(pid)))
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

        int pid() const noexcept { return GetProcessId(_handle.get()); }

    }; // handle_storage

} // namespace

#endif // include guard
