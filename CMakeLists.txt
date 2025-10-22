cmake_minimum_required(VERSION 3.30)

set(PROJECT_LANGUAGES CXX)

project(flob
    VERSION 0.1
    DESCRIPTION "Fast Limit Order Book"
    LANGUAGES ${PROJECT_LANGUAGES}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BIN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/binaries/$<IF:$<CONFIG:Release>,release,$<IF:$<CONFIG:Debug>,debug,other>>")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_subdirectory(flob)
add_subdirectory(samples)
