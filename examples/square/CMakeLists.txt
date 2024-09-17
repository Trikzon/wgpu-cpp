project(triangle)

add_executable(square main.cpp)

target_link_libraries(square PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(square)
