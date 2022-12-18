cmake_minimum_required(VERSION 3.19)
project(LR7)

set(CMAKE_CXX_STANDARD 20)

add_executable(LR7 main.cpp)

target_link_libraries(LR7 wsock32 ws2_32)


