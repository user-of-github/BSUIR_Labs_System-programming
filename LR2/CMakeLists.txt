# CMakeList.txt : CMake project for LR2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("LR2")

# Add source to this project's executable.
add_executable (LR2 "LR2.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LR2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
