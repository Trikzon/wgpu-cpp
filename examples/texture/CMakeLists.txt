project(texture)

add_executable(texture main.cpp)

target_link_libraries(texture PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(texture)
