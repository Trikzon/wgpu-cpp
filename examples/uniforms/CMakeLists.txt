project(uniforms)

add_executable(uniforms main.cpp)

target_link_libraries(uniforms PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(uniforms)
