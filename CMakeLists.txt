cmake_minimum_required(VERSION 3.24)
project(PEA_1_BF_BB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_executable(PEA_1_BF_BB main.cpp BB.cpp BB.h)
