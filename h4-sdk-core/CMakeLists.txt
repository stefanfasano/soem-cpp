cmake_minimum_required(VERSION 3.14)
project(h4-sdk-core LANGUAGES CXX)

# Build settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Import your PID package(s) here
#import_PID_Package(PACKAGE ethercatcpp-core)
#import_PID_Package(PACKAGE pid-threading)
#import_PID_Package(PACKAGE pid-os-utilities)

include_directories(src include apps)

### creating a library
add_library(h4SDKCore SHARED
        include/AbstractIMU.h
        include/AbstractMotorController.h
        include/H4EtherSnacksBoard.h
        include/Master.h
        include/Slove.h
        src/AbstractIMU.cpp
        src/AbstractMotorController.cpp
        src/H4EtherSnacksBoard.cpp
        src/Master.cpp
)

target_link_libraries(h4SDKCore soem)

#bind_PID_Components(LIB h4SDKCore COMPONENTS ethercatcpp-core/ethercatcpp-core)

### creating an executable (h4IMUTestbed)
#add_executable(h4IMUTestbed apps/H4IMUTestbed.cpp)
#target_link_libraries(h4IMUTestbed soem)

### creating an executable (h4IMUTestbed)
add_executable(h4EtherSnacksTestbed apps/H4EtherSnacksTestbed.cpp)
target_link_libraries(h4EtherSnacksTestbed soem h4SDKCore)

install(TARGETS h4SDKCore h4EtherSnacksTestbed
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)