ProjectLog("Host system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

if(
  (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64") 
  OR (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm")
  OR (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
)
    set(WHEELS_ARCH "armv8_a_64")
endif()

if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(WHEELS_ARCH "x86_64")
endif()
