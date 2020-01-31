cmake_minimum_required(VERSION 3.14)
project(Lab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab2 main.cpp particle.h constants.h particle.cpp vec3d.h vec3d.cpp particlesystem.h particlesystem.cpp randdouble.h randdouble.cpp)
include_directories(lib include/GL GL)
taLabet_link_libraries(Lab2 -lOpenGl32 -lfreeglut -lglu32 )