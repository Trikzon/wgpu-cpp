#include <iostream>

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <wgpu.hpp>

constexpr uint32_t WINDOW_WIDTH = 600, WINDOW_HEIGHT = 400;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "wgpu-cpp window example", nullptr, nullptr);

    const wgpu::Instance instance = wgpu::create_instance({});
    const wgpu::Surface surface{glfwGetWGPUSurface(instance.c_ptr(), window)};

    // You should handle the case where the adapter or device is not created.
    const auto adapter = instance.create_adapter({.compatible_surface = &surface,}).value();
    const auto device = adapter.create_device({}).value();

    const auto surface_capabilities = surface.get_capabilities(adapter);

    surface.configure(
    {
        .device = device,
        .format = surface_capabilities.formats[0],
        .usage = wgpu::TextureUsageFlags::RenderAttachment,
        .alpha_mode = wgpu::CompositeAlphaMode::Auto,
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,
        .present_mode = wgpu::PresentMode::Fifo,
    });

    while (!glfwWindowShouldClose(window))
    {
        instance.process_events();
        device.tick();
        glfwPollEvents();

        const auto command_encoder = device.create_command_encoder({.label = "Command Encoder"});

        const auto surface_view = surface.get_current_texture().texture.create_view();

        const auto render_pass = command_encoder.begin_render_pass({
            .label = "Render Pass",
            .color_attachments = std::vector<wgpu::RenderPassColorAttachment>
            {
                {
                    .view = surface_view,
                    .resolve_target = std::nullopt,
                    .load_op = wgpu::LoadOp::Clear,
                    .store_op = wgpu::StoreOp::Store,
                    // The clear color is set to blue here!
                    .clear_value = {0.25f, 0.25f, 0.75f, 1.0f}
                }
            },
        });

        render_pass.end();
        const auto command_buffer = command_encoder.finish({.label = "Command Buffer"});
        device.get_queue().submit({command_buffer});

        surface.present();
    }

    glfwTerminate();

    return 0;
}
