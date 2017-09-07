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
