cmake_minimum_required(VERSION 3.28)

project(wgpu_cpp CXX)

option(WPGU_CPP_BUILD_EXAMPLES "Build examples" OFF)
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(WPGU_CPP_BUILD_EXAMPLES ON)
endif()

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
        src/private/wgpu.cpp
        src/public/wgpu.hpp
)

add_library(wgpu_cpp STATIC ${SOURCES})
target_include_directories(wgpu_cpp PUBLIC src/public)

# WGPU fails to compile on Mac and Dawn fails to compile on Windows currently.
# This should be fixed in a Dawn update.
if (WIN32)
    set(WEBGPU_BACKEND WGPU)
elseif (APPLE)
    set(WEBGPU_BACKEND DAWN)
endif()

add_subdirectory(vendor/WebGPU-distribution)
target_link_libraries(wgpu_cpp PUBLIC webgpu)

if (WPGU_CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

target_copy_webgpu_binaries(wgpu_cpp)
