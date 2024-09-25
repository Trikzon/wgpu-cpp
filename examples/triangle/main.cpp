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
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "wgpu-cpp triangle example", nullptr, nullptr);

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

    constexpr auto shader_src = "@vertex\n"
                                "fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {\n"
                                "   var p = vec2f(0.0, 0.0);\n"
                                "   if (in_vertex_index == 0u) {\n"
                                "       p = vec2f(-0.5, -0.5);\n"
                                    "} else if (in_vertex_index == 1u) {\n"
                                "       p = vec2f(0.5, -0.5);\n"
                                "   } else {\n"
                                "       p = vec2f(0.0, 0.5);\n"
                                "   }\n"
                                "   return vec4f(p, 0.0, 1.0);\n"
                                "}\n"
                                "\n"
                                "@fragment\n"
                                "fn fs_main() -> @location(0) vec4f {\n"
                                "   return vec4f(0.0, 0.4, 1.0, 1.0);\n"
                                "}\n";

    constexpr wgpu::ShaderModuleWGSLDescriptor wgsl_descriptor
    {
        .chain = wgpu::ChainedStruct
        {
            .next_in_chain = nullptr,
            .s_type = wgpu::SType::ShaderModuleWGSLDescriptor,
        },
        .code = shader_src
    };

    const auto shader_module = device.create_shader_module({
        .next_in_chain = &wgsl_descriptor.chain,
        .label = "Shader Module",
    });

    const auto render_pipeline = device.create_render_pipeline(wgpu::RenderPipelineDescriptor
    {
        .label = "Render Pipeline",
        .vertex = wgpu::VertexState
        {
            .module = shader_module,
            .entry_point = "vs_main",
            .constants = {},
            .buffers = {},
        },
        .primitive = wgpu::PrimitiveState
        {
            .topology = wgpu::PrimitiveTopology::TriangleList,
            .strip_index_format = wgpu::IndexFormat::Undefined,
            .front_face = wgpu::FrontFace::CCW,
            .cull_mode = wgpu::CullMode::None,
        },
        .depth_stencil = std::nullopt,
        .multisample = wgpu::MultisampleState
        {
            .count = 1,
            .mask = ~0u,
            .alpha_to_coverage_enabled = false,
        },
        .fragment = wgpu::FragmentState
        {
            .module = shader_module,
            .entry_point = "fs_main",
            .constants = {},
            .targets = std::vector<wgpu::ColorTargetState>
            {
                {
                    .format = surface.get_capabilities(adapter).formats[0],
                    .blend = wgpu::BlendState
                    {
                        .color = wgpu::BlendComponent
                        {
                            .operation = wgpu::BlendOperation::Add,
                            .src_factor = wgpu::BlendFactor::SrcAlpha,
                            .dst_factor = wgpu::BlendFactor::OneMinusSrcAlpha,
                        },
                        .alpha = wgpu::BlendComponent
                        {
                            .operation = wgpu::BlendOperation::Add,
                            .src_factor = wgpu::BlendFactor::Zero,
                            .dst_factor = wgpu::BlendFactor::One,
                        },
                    },
                    .write_mask = wgpu::ColorWriteMaskFlags::All,
                }
            },
        }
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
                    // The clear color is set to red here!
                    .clear_value = {0.75f, 0.25f, 0.25f, 1.0f}
                }
            },
        });

        render_pass.set_pipeline(render_pipeline);
        render_pass.draw(3, 1, 0, 0);

        render_pass.end();
        const auto command_buffer = command_encoder.finish({.label = "Command Buffer"});
        device.get_queue().submit({command_buffer});

        surface.present();
    }

    glfwTerminate();

    return 0;
}
