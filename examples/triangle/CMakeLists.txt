project(triangle)

add_executable(triangle main.cpp)

target_link_libraries(triangle PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(triangle)
