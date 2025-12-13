cmake_minimum_required(VERSION 3.14)
project(h4-sdk)

# Set install prefix (optional)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(SOEM_BUILD_SAMPLES ON CACHE BOOL "Build SOEM samples")
add_subdirectory(h4-sdk-core)
add_subdirectory(SOEM)