project(buffers)

add_executable(buffers main.cpp)

target_link_libraries(buffers PRIVATE wgpu_cpp)
target_copy_webgpu_binaries(buffers)
