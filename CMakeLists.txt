cmake_minimum_required(VERSION 3.9)
project(wheels)

include(cmake/BuildTypes.cmake)
include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Platform.cmake)
include(cmake/Processor.cmake)

add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(wheels)

option(WHEELS_TESTS "Enable wheels tests" OFF)
option(WHEELS_DEVELOPER, "Wheels development mode" OFF)

if(WHEELS_TESTS OR WHEELS_DEVELOPER)
    add_subdirectory(tests)
endif()
