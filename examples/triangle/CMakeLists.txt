project(triangle)

add_executable(triangle_with_buffers main.cpp)

target_link_libraries(triangle_with_buffers PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(triangle_with_buffers)
