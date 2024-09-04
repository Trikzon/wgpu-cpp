project(examples)

if (NOT EMSCRIPTEN)
    add_subdirectory(vendor/glfw)
else()
    add_library(glfw INTERFACE)
    target_compile_options(glfw INTERFACE -sUSE_GLFW=3)
endif()

add_subdirectory(vendor/glfw3webgpu)

add_subdirectory(window)
