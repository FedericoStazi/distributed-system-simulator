cmake_minimum_required(VERSION 3.17)
project(distributed_system_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_library(dssim dssim.h)
set_target_properties(dssim PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(examples)