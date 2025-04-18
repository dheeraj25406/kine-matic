cmake_minimum_required(VERSION 3.10)
project(KineMatic)

add_executable(kinematic main.cc window.cc)
target_link_libraries(kinematic PRIVATE sdl2::sdl2)