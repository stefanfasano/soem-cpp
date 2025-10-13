cmake_minimum_required(VERSION 3.14)
project(h4-sdk)

# Adjust path if needed
set(PATH_TO_PID_WS ${CMAKE_SOURCE_DIR}/pid-workspace CACHE PATH "Path to the root folder of PID workspace")

if(NOT EXISTS ${PATH_TO_PID_WS})
    message(FATAL_ERROR "ERROR path ${PATH_TO_PID_WS} does not exist !!")
endif()

include(${PATH_TO_PID_WS}/Use_PID.cmake)
import_PID_Workspace(PATH ${PATH_TO_PID_WS})

# Set install prefix (optional)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)



add_subdirectory(h4-sdk-core)