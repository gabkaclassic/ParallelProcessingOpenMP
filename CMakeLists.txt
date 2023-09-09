cmake_minimum_required(VERSION 3.25)
project(untitled)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

#set(SOURCE_FILES lab_1/main.cpp)
#set(SOURCE_FILES lab_2/main.cpp)
set(SOURCE_FILES lab_3/main.cpp)
add_executable(openmp_test_clion ${SOURCE_FILES})
