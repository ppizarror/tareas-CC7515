# Propiedades del make
cmake_minimum_required(VERSION 3.12)
project(tarea_01)
set(CMAKE_CXX_STANDARD 14)

# Define tests
add_executable(TEST-HALF-EDGE tests/half_edge/test_half_edge.cpp half_edge/face.h half_edge/vertex.h half_edge/h_edge.cpp half_edge/h_edge.h)
add_executable(TEST-POINT tests/elements/test_point.cpp elements/point.h)
add_executable(TEST-VECTOR tests/elements/test_vector.cpp elements/vector.h)