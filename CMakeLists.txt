cmake_minimum_required(VERSION 3.14)
project(h4-sdk)

add_subdirectory(h4-sdk-core)

set(PATH_TO_PID_WS ${CMAKE_CURRENT_SOURCE_DIR}/pid-workspace CACHE PATH "Path to the root folder of PID workspace")

if(NOT EXISTS ${PATH_TO_PID_WS})
    message("ERROR path ${PATH_TO_PID_WS} does not exist !!")
    return()
endif()

include(${PATH_TO_PID_WS}/Use_PID.cmake)
import_PID_Workspace(PATH ${PATH_TO_PID_WS})

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

import_PID_Package(PACKAGE ethercatcpp-core)
#import_PID_Package(PACKAGE boost VERSION 1.64.0)

include_directories(src include)

### creating a library
add_library(mylib SHARED
        include/lib.h
        src/lib.cpp
        src/main.cpp)
bind_PID_Components(LIB mylib COMPONENTS ethercatcpp-core/ethercatcpp-core)
#
#### creating an executable
add_executable(myapp src/main.cpp)
bind_PID_Components(EXE myapp COMPONENTS ethercatcpp-core/ethercatcpp-core)
#bind_Local_Component(EXE myapp COMPONENT mylib)


install(TARGETS mylib myapp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)