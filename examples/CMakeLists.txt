project(examples)

if (NOT EMSCRIPTEN)
    if (NOT TARGET glfw)
        add_subdirectory(vendor/glfw)
    endif()
else()
    add_library(glfw INTERFACE)
    target_compile_options(glfw INTERFACE -sUSE_GLFW=3)
endif()

add_subdirectory(vendor/glfw3webgpu)

add_subdirectory(buffers)
add_subdirectory(triangle)
add_subdirectory(window)
