#include <iostream>

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <wgpu.hpp>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(600, 400, "Window Example", nullptr, nullptr);

    constexpr wgpu::InstanceDescriptor instance_descriptor{};
    const wgpu::Instance instance = create_instance(instance_descriptor);

    const wgpu::Surface surface{glfwGetWGPUSurface(instance.c_ptr(), window)};

    const auto adapter = instance.create_adapter({
        .compatible_surface = &surface,
    }).value();

    const auto device = adapter.create_device({}).value();

    const auto surface_capabilities = surface.get_capabilities(adapter);

    surface.configure({
        .next_in_chain = nullptr,
        .device = device,
        .format = surface_capabilities.formats[0],
        .usage = wgpu::TextureUsageFlags::RenderAttachment,
        .view_formats = {},
        .alpha_mode = wgpu::CompositeAlphaMode::Auto,
        .width = 600,
        .height = 400,
        .present_mode = wgpu::PresentMode::Fifo,
    });

    std::cout << "Hello, world!" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        instance.process_events();
        glfwPollEvents();

        const auto surface_texture = surface.get_current_texture().texture;
        const auto surface_view = surface_texture.create_view();

        surface.present();
    }

    glfwTerminate();

    return 0;
}
