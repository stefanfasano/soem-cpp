//
// Created by srfas on 10/9/2025.
//

#include <lib.h>

#include<pid/rpath.h>

std::string get_Absolute_Path(const std::string& rel_path){
    return (PID_PATH(rel_path));
}