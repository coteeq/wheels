cmake_minimum_required(VERSION 3.14)

project(wheels)

include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

include(cmake/CompileOptions.cmake)
include(cmake/Platform.cmake)
include(cmake/Processor.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(wheels)

option(WHEELS_TESTS "Enable wheels tests" OFF)
option(WHEELS_DEVELOPER "Wheels development mode" OFF)

if(WHEELS_TESTS OR WHEELS_DEVELOPER)
    add_subdirectory(tests)
endif()
