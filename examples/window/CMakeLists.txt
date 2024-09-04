project(window)

add_executable(window main.cpp)

target_link_libraries(window PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(window)
