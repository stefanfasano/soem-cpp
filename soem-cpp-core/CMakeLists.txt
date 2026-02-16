cmake_minimum_required(VERSION 3.14)
project(soem-cpp-core LANGUAGES CXX)

# Build settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src include apps examples)

### creating a library
add_library(soemCppCore SHARED
        include/AbstractIMU.h
        include/AbstractMotorController.h
        include/Master.h
        include/Slove.h
        src/AbstractIMU.cpp
        src/AbstractMotorController.cpp
        src/Master.cpp
)

target_link_libraries(soemCppCore soem)

### creating an executable (h4IMUTestbed)
add_executable(h4EtherSnacksTestbed examples/H4EtherSnacksTestbed.cpp examples/H4EtherSnacksBoard.cpp examples/H4EtherSnacksBoard.h)
target_link_libraries(h4EtherSnacksTestbed soemCppCore)

install(TARGETS soemCppCore h4EtherSnacksTestbed
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)