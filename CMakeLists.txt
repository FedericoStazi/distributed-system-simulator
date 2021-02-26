cmake_minimum_required(VERSION 3.17)
project(distributed_system_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_library(distributed_system_simulator include/dssim.h)
set_target_properties(distributed_system_simulator PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test_main tests/JobsSchedulerTest.cpp)

target_link_libraries(test_main distributed_system_simulator)