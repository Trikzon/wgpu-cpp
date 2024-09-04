cmake_minimum_required(VERSION 3.28)

project(wgpu_cpp CXX)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
        src/private/wgpu.cpp
        src/public/wgpu.hpp
)

add_library(wgpu_cpp STATIC ${SOURCES})
target_include_directories(wgpu_cpp PUBLIC src/public)

add_subdirectory(vendor/WebGPU-distribution)

target_link_libraries(wgpu_cpp PUBLIC
        -static-libgcc -static-libstdc++
        webgpu)

add_subdirectory(examples)

target_copy_webgpu_binaries(wgpu_cpp)
