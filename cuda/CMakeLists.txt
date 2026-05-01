cmake_minimum_required(VERSION 3.24)

project(CudaImplNBody LANGUAGES C CXX CUDA)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# 80,90 for delta, 86 for 3070
set(CMAKE_CUDA_ARCHITECTURES native)


add_executable(cuda-impl main.cpp src/drawing.cpp src/maths.cu src/physics.cu src/bodies.cpp kernels/kernels.cu)

set_target_properties(cuda-impl PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_include_directories(cuda-impl PRIVATE include kernels)
target_link_libraries(cuda-impl PRIVATE raylib m pthread X11)
