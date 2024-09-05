#include <glfw3webgpu.h>
#include <iostream>

#include <GLFW/glfw3.h>
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

    // surface.configure({
    //     .next_in_chain = nullptr,
    //     .device = nullptr,
    //     .format = wgpu::TextureFormat::Undefined,
    //     .usage = wgpu::TextureUsageFlags::RenderAttachment,
    //     .view_formats = {},
    //     .alpha_mode = wgpu::CompositeAlphaMode::Auto,
    //     .width = 600,
    //     .height = 400,
    //     .present_mode = wgpu::PresentMode::Fifo,
    // });

    const wgpu::RequestAdapterOptions adapter_options
    {
        .compatible_surface = &surface,
    };
    const auto adapter = instance.request_adapter(adapter_options).value();

    std::cout << "Hello, world!" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
