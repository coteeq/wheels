cmake_minimum_required(VERSION 3.5)

# --------------------------------------------------------------------

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)


file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_ASM_SOURCES ${LIB_PATH}/*.S)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

enable_language(ASM)

add_library(wheels STATIC ${LIB_CXX_SOURCES} ${LIB_ASM_SOURCES} ${LIB_HEADERS})
target_include_directories(wheels PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(wheels PUBLIC pthread)

# --------------------------------------------------------------------

# Test framework settings

option(WHEELS_FORK_TESTS "Fork tests" ON)
option(WHEELS_NO_TEST_TIME_LIMIT "Disable time limits for tests" OFF)

set(TEST_FRAMEWORK_DEFINITIONS "")

if(WHEELS_FORK_TESTS)
    message(STATUS "Fork tests")
    list(APPEND TEST_FRAMEWORK_DEFINITIONS WHEELS_FORK_TESTS=1)
endif()

if (WHEELS_NO_TEST_TIME_LIMIT)
    message(STATUS "Disable test time limits")
    list(APPEND TEST_FRAMEWORK_DEFINITIONS WHEELS_NO_TEST_TIME_LIMIT=1)
endif()

target_compile_definitions(wheels PUBLIC ${TEST_FRAMEWORK_DEFINITIONS})

# --------------------------------------------------------------------

# Linters

if(CLANG_FORMAT_TOOL)
    add_clang_format_target(
            wheels_clang_format
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${LIB_HEADERS} ${LIB_CXX_SOURCES})
endif()

if(CLANG_TIDY_TOOL)
    add_clang_tidy_target(
            wheels_clang_tidy
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${LIB_INCLUDE_PATH}
            ${LIB_HEADERS} ${LIB_CXX_SOURCES})
endif()
