#ifndef JM_PROCESS_HANDLE_CONFIG_HPP
#define JM_PROCESS_HANDLE_CONFIG_HPP

#if   defined(_WIN32)
    #define JM_PROCESS_HANDLE_WINDOWS
#elif defined(__linux__)
    #define JM_PROCESS_HANDLE_LINUX
#elif defined(__APPLE__)
    #define JM_PROCESS_HANDLE_APPLE
#else
    #error unknown operating system
#endif

#endif // include guard
