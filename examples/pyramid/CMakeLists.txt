project(pyramid)

add_executable(pyramid main.cpp)

target_link_libraries(pyramid PRIVATE glfw glfw3webgpu wgpu_cpp)
target_copy_webgpu_binaries(pyramid)
