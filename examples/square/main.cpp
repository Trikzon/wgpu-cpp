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
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "wgpu-cpp triangle_with_buffers example",
        nullptr, nullptr);

    const wgpu::Instance instance = wgpu::create_instance({});
    const wgpu::Surface surface{glfwGetWGPUSurface(instance.c_ptr(), window)};

    // You should handle the case where the adapter or device is not created.
    const auto adapter = instance.create_adapter({.compatible_surface = &surface,}).value();
    const auto device = adapter.create_device({}).value();
    const auto queue = device.get_queue();

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

    const std::vector<float> point_data
    {
        // x,    y,     r,   g,   b
        -0.5, -0.5,   1.0, 0.0, 0.0, // Point #0
        +0.5, -0.5,   0.0, 1.0, 0.0, // Point #1
        +0.5, +0.5,   0.0, 0.0, 1.0, // Point #2
        -0.5, +0.5,   1.0, 1.0, 0.0, // Point #3
    };

    const std::vector<uint16_t> index_data
    {
        0, 1, 2, // Triangle #0 connects points #0, #1, and #2.
        0, 2, 3, // Triangle #1 connects points #0, #2, and #3.
    };

    const auto point_buffer = device.create_buffer(
    {
        .label = "Point Buffer",
        .usage = wgpu::BufferUsageFlags::CopyDst | wgpu::BufferUsageFlags::Vertex,
        .mapped_at_creation = false,
        .size = point_data.size() * sizeof(float)
    });
    queue.write_buffer(point_buffer, 0, point_data);

    const auto index_buffer = device.create_buffer(
    {
        .label = "Index Buffer",
        .usage = wgpu::BufferUsageFlags::CopyDst | wgpu::BufferUsageFlags::Index,
        .mapped_at_creation = false,
        .size = index_data.size() * sizeof(uint16_t),
    });
    queue.write_buffer(index_buffer, 0, index_data);

    constexpr auto shader_src = "struct VertexInput {\n"
                                "   @location(0) position: vec2f,\n"
                                "   @location(1) color: vec3f,\n"
                                "};\n"
                                "\n"
                                "struct VertexOutput {\n"
                                "   @builtin(position) position: vec4f,\n"
                                "   @location(0) color: vec3f,\n"
                                "}\n"
                                "\n"
                                "@vertex\n"
                                "fn vs_main(in: VertexInput) -> VertexOutput {\n"
                                "   var out: VertexOutput;\n"
                                "   let ratio = 600.0 / 400.0;"
                                "   out.position = vec4f(in.position.x, in.position.y * ratio, 0.0, 1.0);\n"
                                "   out.color = in.color;\n"
                                "   return out;\n"
                                "}\n"
                                "\n"
                                "@fragment\n"
                                "fn fs_main(in: VertexOutput) -> @location(0) vec4f {\n"
                                "   return vec4f(in.color, 1.0);\n"
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
        .vertex = wgpu::VertexState
        {
            .module = shader_module,
            .entry_point = "vs_main",
            .constants = {},
            .buffers = std::vector<wgpu::VertexBufferLayout>
            {
                {
                    .array_stride = 5 * sizeof(float),
                    .step_mode = wgpu::VertexStepMode::Vertex,
                    .attributes = std::vector<wgpu::VertexAttribute>
                    {
                        {
                            .format = wgpu::VertexFormat::Float32x2,
                            .offset = 0,
                            .shader_location = 0,
                        },
                        {
                            .format = wgpu::VertexFormat::Float32x3,
                            .offset = 2 * sizeof(float),
                            .shader_location = 1,
                        }
                    }
                }
            },
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
                    .clear_value = {0.15f, 0.15f, 0.15f, 1.0f}
                }
            },
        });

        render_pass.set_pipeline(render_pipeline);
        render_pass.set_vertex_buffer(0, point_buffer, 0, point_buffer.get_size());
        render_pass.set_index_buffer(index_buffer, wgpu::IndexFormat::Uint16, 0, index_buffer.get_size());
        render_pass.draw_indexed(index_data.size(), 1, 0, 0, 0);

        render_pass.end();
        const auto command_buffer = command_encoder.finish({.label = "Command Buffer"});
        queue.submit({command_buffer});

        surface.present();
    }

    glfwTerminate();

    return 0;
}
