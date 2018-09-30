# Propiedades del make
cmake_minimum_required(VERSION 3.12)
project(tarea_01)
set(CMAKE_CXX_STANDARD 14)

# Define tests
add_executable(TEST-POINT tests/point.cpp elements/point.h)
add_executable(TEST-VECTOR tests/vector.cpp elements/vector.h)