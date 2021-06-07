cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(SlidingPuzzleSolver)

add_executable(${PROJECT_NAME} src/main.cpp src/SlidingPuzzleSolver.h src/Vector.h src/State.h)