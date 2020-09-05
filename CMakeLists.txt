cmake_minimum_required(VERSION 3.10)

# set the project name
project(cgp)

# add the executable
add_executable(cgp main.cpp 
reference_bits.cpp my_exception.hpp reference_bits.hpp 
utils.hpp utils.cpp
parameters.cpp
parameters.hpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)