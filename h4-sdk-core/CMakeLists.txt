cmake_minimum_required(VERSION 3.14)
project(h4-sdk-core LANGUAGES CXX)

# Build settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#set(PATH_TO_PID_WS ${CMAKE_SOURCE_DIR}/pid-workspace CACHE PATH "Path to the root folder of PID workspace")
#
#if(NOT EXISTS ${PATH_TO_PID_WS})
#    message("ERROR path ${PATH_TO_PID_WS} does not exist !!")
#    return()
#endif()
#
#include(${PATH_TO_PID_WS}/Use_PID.cmake)
#import_PID_Workspace(PATH ${PATH_TO_PID_WS})
#
#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
#
#import_PID_Package(PACKAGE ethercatcpp-core)
#import_PID_Package(PACKAGE boost VERSION 1.64.0)

# Import your PID package(s) here
import_PID_Package(PACKAGE ethercatcpp-core)
import_PID_Package(PACKAGE pid-threading)
import_PID_Package(PACKAGE pid-os-utilities)

include_directories(src include apps)

### creating a library
add_library(h4SDKCore SHARED
        include/abstract_imu.h
        include/abstract_motor_controller.h
        include/h4_ethersnacks_board.h
        include/h4_imu.h
        src/abstract_imu.cpp
        src/abstract_motor_controller.cpp
        src/h4_ethersnacks_board.cpp
        src/h4_imu.cpp
)
bind_PID_Components(LIB h4SDKCore COMPONENTS ethercatcpp-core/ethercatcpp-core)

### creating an executable (h4IMUTestbed)
add_executable(h4IMUTestbed apps/h4IMUTestbed.cpp)
bind_PID_Components(EXE h4IMUTestbed COMPONENTS pid-threading/synchro pid-os-utilities/realtime pid-os-utilities/signal-manager)
bind_Local_Component(EXE h4IMUTestbed COMPONENT h4SDKCore)

### creating an executable (h4IMUTestbed)
add_executable(h4EtherSnacksTestbed apps/h4EtherSnacksTestbed.cpp)
bind_PID_Components(EXE h4EtherSnacksTestbed COMPONENTS pid-threading/synchro pid-os-utilities/realtime pid-os-utilities/signal-manager)
bind_Local_Component(EXE h4EtherSnacksTestbed COMPONENT h4SDKCore)

install(TARGETS h4SDKCore h4IMUTestbed h4EtherSnacksTestbed
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)