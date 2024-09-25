project(dynamic_uniforms)

add_executable(dynamic_uniforms main.cpp)

target_link_libraries(dynamic_uniforms PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(dynamic_uniforms)
