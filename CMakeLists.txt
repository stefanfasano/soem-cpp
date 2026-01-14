cmake_minimum_required(VERSION 3.14)
project(h4-sdk)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

#Enforces standard c++ (more flexible for other compilers/setups)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#No compiler-specific extensions (more flexible for other compilers/setups)
set(CMAKE_CXX_EXTENSIONS OFF)

#Sets extra warning printouts in debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")

# Set install prefix (optional)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(SOEM_BUILD_SAMPLES ON CACHE BOOL "Build SOEM samples")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(SOEM)
add_subdirectory(h4-sdk-core)

