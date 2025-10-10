//
// Created by srfas on 10/9/2025.
//

#include <lib.h>
#include <boost/filesystem.h>
#include <iostream>
using namespace boost;

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout<<"you must input a path !"<<std::endl;
        exit(0);
    }

    path p(get_Absolute_Path(argv[1]));
    if(exists(p)){
        std::cout<<"path exists"<<std::endl;
        return(0);
    }
    else{
        std::cout<<"path does NOT exist"<<std::endl;
        return(-1);
    }
}