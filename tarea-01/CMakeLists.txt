# Propiedades del make
cmake_minimum_required(VERSION 3.10)
project(tarea_01)
set(CMAKE_CXX_STANDARD 14)

# Define tests
add_executable(TEST-HEDGE-OFF tests/half_edge/test_load_off.cpp half_edge/loff/load_off.h)
add_executable(TEST-HEDGE tests/half_edge/test_half_edge.cpp half_edge/face.h half_edge/vertex.h half_edge/h_edge.h)
add_executable(TEST-POINT tests/elements/test_point.cpp elements/point.h)
add_executable(TEST-VECTOR tests/elements/test_vector.cpp elements/vector.h)
add_executable(TEST-SEGMENT tests/elements/test_segment.cpp elements/segment.h)
add_executable(TEST-POLYGON tests/elements/test_polygon.cpp elements/polygon.h)