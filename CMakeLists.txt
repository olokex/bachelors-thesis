cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(bachelors-thesis VERSION 1.0)

# add the executable
add_executable(cgp
    src/cgp/main.cpp
    src/cgp/parameters.cpp
    src/cgp/circuit.cpp
    src/cgp/circuit.hpp
    src/cgp/parameters.hpp
    src/cgp/function.hpp
    src/cgp/cell.hpp
    src/reference_bits.hpp
    src/reference_bits.cpp
    src/utils.hpp
    src/utils.cpp
)

# add the executable
add_executable(anf
    #src/cgp/main.cpp
    #src/cgp/parameters.hpp
    src/anf/parameters.cpp
    #src/reference_bits.hpp
    #src/reference_bits.cpp
    #src/utils.hpp
    #src/utils.cpp
)