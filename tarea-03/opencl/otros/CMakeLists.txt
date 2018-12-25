project(T3-CC7517-OPENCL)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS 1)
set(
        CUDA_PATH
        "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.0"
)
include(FindOpenCL.cmake)
find_package(OPENCL REQUIRED)
include_directories(${OPENCL_INCLUDE_DIRS})

add_executable(MAIN main.cpp)
target_link_libraries(MAIN ${OPENCL_LIBRARIES})