cmake_minimum_required(VERSION 3.14)
project(Lab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab1 main.cpp)
target_link_libraries(Lab1 -lOpenGl32 -lfreeglut -lglu32)

