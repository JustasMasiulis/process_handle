cmake_minimum_required(VERSION 3.3)
project(process_handle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories (${PROJECT_SOURCE_DIR}/include)
add_library (process_handle INTERFACE)
target_include_directories (process_handle INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_sources (process_handle INTERFACE ${PROJECT_SOURCE_DIR}/include/process_handle.hpp)

enable_testing()
add_subdirectory (test)
