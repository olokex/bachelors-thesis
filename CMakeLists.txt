cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")
#target_link_libraries(MyExeOrLibrary PUBLIC Boost::filesystem)
#
#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#find_package(Boost 1.45.0 COMPONENTS Boost::dynamic_bitset)
#
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS}) 
#    #add_executable(progname file1.cxx file2.cxx) 
#    #target_link_libraries(cgp ${Boost_LIBRARIES})
#endif()

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
    #${Boost_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)