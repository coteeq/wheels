cmake_minimum_required(VERSION 3.5)

message(STATUS "Wheels tests")

# List sources

set(RUN_TESTS_MAIN "./run_tests.cpp")
file(GLOB TESTS "./test*.cpp")

# All tests target

add_executable(wheels_all_tests ${RUN_TESTS_MAIN} ${TESTS})
target_link_libraries(wheels_all_tests wheels)
