cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# set the project name
project(cgp VERSION 1.0)

# add the executable
add_executable(cgp
    src/main.cpp
    src/parameters.hpp
    src/utils.hpp
    src/reference_bits.hpp
    src/my_exception.hpp
    src/parameters.cpp
    src/utils.cpp
    src/reference_bits.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)