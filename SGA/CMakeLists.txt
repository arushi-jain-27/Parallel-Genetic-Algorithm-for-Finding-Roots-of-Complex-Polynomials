cmake_minimum_required(VERSION 3.0)
project(Polynomial_Root_Finding_pGA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        World/Individual.cpp
        World/Individual.h
        World/Population.cpp
        World/Population.h
        World.h
        Local.h
        main.cpp
        Local.cpp
        Worker.cpp
        Worker.h
        World/Polynomial.cpp
        World/Polynomial.h)

add_executable(Polynomial_Root_Finding_pGA ${SOURCE_FILES})
