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
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "wgpu-cpp pyramid example",
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
        // x,    y,    z,      r,   g,   b
        // The base of the pyramid.
        -0.5, -0.5, -0.3,    1.0, 0.0, 1.0,
        +0.5, -0.5, -0.3,    1.0, 0.0, 1.0,
        +0.5, +0.5, -0.3,    1.0, 0.0, 1.0,
        -0.5, +0.5, -0.3,    1.0, 0.0, 1.0,
        // The tip of the pyramid.
        +0.0, +0.0, +0.5,    0.5, 0.0, 0.5,
    };

    const std::vector<uint16_t> index_data
    {
        // Base
        0, 1, 2,
        0, 2, 3,
        // Sides
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
    };

    const auto point_buffer = device.create_buffer(
    {
        .label = "Point Buffer",
        .usage = wgpu::BufferUsageFlags::CopyDst | wgpu::BufferUsageFlags::Vertex,
        .mapped_at_creation = false,
        .size = point_data.size() * sizeof(float),
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

    const auto uniform_buffer = device.create_buffer(
    {
        .label = "Uniform Buffer",
        .usage = wgpu::BufferUsageFlags::CopyDst | wgpu::BufferUsageFlags::Uniform,
        .mapped_at_creation = false,
        .size = sizeof(float),
    });

    const auto bind_group_layout = device.create_bind_group_layout(
    {
        .label = "Bind Group Layout",
        .entries = std::vector<wgpu::BindGroupLayoutEntry>
        {
            {
                .binding = 0,
                .visibility = wgpu::ShaderStageFlags::Vertex,
                .buffer = wgpu::BufferBindingLayout
                {
                    .type = wgpu::BufferBindingType::Uniform,
                    .has_dynamic_offset = false,
                    .min_binding_size = sizeof(float),
                }
            }
        }
    });

    const auto bind_group = device.create_bind_group(
    {
        .label = "Bind Group",
        .layout = bind_group_layout,
        .entries = std::vector<wgpu::BindGroupEntry>
        {
            {
                .binding = 0,
                .buffer = uniform_buffer,
                .offset = 0,
                .size = sizeof(float),
            }
        }
    });

    const auto depth_texture = device.create_texture(wgpu::TextureDescriptor
    {
        .label = "Depth Texture",
        .usage = wgpu::TextureUsageFlags::RenderAttachment,
        .dimension = wgpu::TextureDimension::_2D,
        .size = {WINDOW_WIDTH, WINDOW_HEIGHT, 1},
        .format = wgpu::TextureFormat::Depth24Plus,
        .mip_level_count = 1,
        .sample_count = 1,
        .view_formats = std::vector
        {
            wgpu::TextureFormat::Depth24Plus,
        },
    });

    const auto depth_texture_view = depth_texture.create_view(wgpu::TextureViewDescriptor
    {
        .label = "Depth Texture View",
        .format = depth_texture.get_format(),
        .dimension = wgpu::TextureViewDimension::_2D,
        .base_mip_level = 0,
        .mip_level_count = 1,
        .base_array_layer = 0,
        .array_layer_count = 1,
        .aspect = wgpu::TextureAspect::DepthOnly,
    });

    constexpr auto shader_src = "struct VertexInput {\n"
                                "    @location(0) position: vec3f,\n"
                                "    @location(1) color: vec3f,\n"
                                "};\n"
                                "\n"
                                "struct VertexOutput {\n"
                                "    @builtin(position) position: vec4f,\n"
                                "    @location(0) color: vec3f,\n"
                                "}\n"
                                "\n"
                                "@group(0) @binding(0) var<uniform> uTime: f32;\n"
                                "\n"
                                "@vertex\n"
                                "fn vs_main(in: VertexInput) -> VertexOutput {\n"
                                "    var out: VertexOutput;\n"
                                "\n"
                                "    let alpha = cos(uTime);\n"
                                "    let beta = sin(uTime);\n"
                                "    var position = vec3f(\n"
                                "        in.position.x,\n"
                                "        alpha * in.position.y + beta * in.position.z,\n"
                                "        alpha * in.position.z - beta * in.position.y,\n"
                                "    );\n"
                                "\n"
                                "    let ratio = 600.0 / 400.0;\n"
                                "    out.position = vec4f(position.x, position.y * ratio, position.z * 0.5 + 0.5, 1.0);\n"
                                "\n"
                                "    out.color = in.color;\n"
                                "\n"
                                "    return out;\n"
                                "}\n"
                                "\n"
                                "@fragment\n"
                                "fn fs_main(in: VertexOutput) -> @location(0) vec4f {\n"
                                "    return vec4f(in.color, 1.0);\n"
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

    const auto pipeline_layout = device.create_pipeline_layout(
    {
        .label = "Pipeline Layout",
        .bind_group_layouts = std::vector
        {
            bind_group_layout,
        },
    });

    const auto render_pipeline = device.create_render_pipeline(wgpu::RenderPipelineDescriptor
    {
        .label = "Render Pipeline",
        .layout = pipeline_layout,
        .vertex = wgpu::VertexState
        {
            .module = shader_module,
            .entry_point = "vs_main",
            .constants = {},
            .buffers = std::vector<wgpu::VertexBufferLayout>
            {
                {
                    .array_stride = 6 * sizeof(float),
                    .step_mode = wgpu::VertexStepMode::Vertex,
                    .attributes = std::vector<wgpu::VertexAttribute>
                    {
                        {
                            .format = wgpu::VertexFormat::Float32x3,
                            .offset = 0,
                            .shader_location = 0,
                        },
                        {
                            .format = wgpu::VertexFormat::Float32x3,
                            .offset = 3 * sizeof(float),
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
        .depth_stencil = wgpu::DepthStencilState
        {
            .format = depth_texture.get_format(),
            .depth_write_enabled = true,
            .depth_compare = wgpu::CompareFunction::Less,
            .stencil_read_mask = 0,
            .stencil_write_mask = 0,
        },
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

        queue.write_buffer(uniform_buffer, 0, static_cast<float>(glfwGetTime()));

        const auto command_encoder = device.create_command_encoder({.label = "Command Encoder"});

        const auto surface_view = surface.get_current_texture().texture.create_view();

        const auto render_pass = command_encoder.begin_render_pass(wgpu::RenderPassDescriptor
        {
            .label = "Render Pass",
            .color_attachments = std::vector<wgpu::RenderPassColorAttachment>
            {
                {
                    .view = surface_view,
                    .resolve_target = std::nullopt,
                    .load_op = wgpu::LoadOp::Clear,
                    .store_op = wgpu::StoreOp::Store,
                    .clear_value = {0.15f, 0.15f, 0.15f, 1.0f},
                },
            },
            .depth_stencil_attachment = wgpu::RenderPassDepthStencilAttachment
            {
                .view = depth_texture_view,
                .depth_load_op = wgpu::LoadOp::Clear,
                .depth_store_op = wgpu::StoreOp::Store,
                .depth_clear_value = 1.0f,
                .depth_read_only = false,
#ifdef WEBGPU_BACKEND_DAWN
                .stencil_load_op = wgpu::LoadOp::Undefined,
                .stencil_store_op = wgpu::StoreOp::Undefined,
                .stencil_clear_value = static_cast<uint32_t>(std::numeric_limits<float>::quiet_NaN()),
#endif
#ifdef WEBGPU_BACKEND_WGPU
                .stencil_load_op = wgpu::LoadOp::Clear,
                .stencil_store_op = wgpu::StoreOp::Store,
                .stencil_clear_value = 0,
#endif
                .stencil_read_only = true,
            },
        });

        render_pass.set_pipeline(render_pipeline);
        render_pass.set_vertex_buffer(0, point_buffer, 0, point_buffer.get_size());
        render_pass.set_index_buffer(index_buffer, wgpu::IndexFormat::Uint16, 0, index_buffer.get_size());
        render_pass.set_bind_group(0, bind_group);
        render_pass.draw_indexed(index_data.size(), 1, 0, 0, 0);

        render_pass.end();
        const auto command_buffer = command_encoder.finish({.label = "Command Buffer"});
        queue.submit({command_buffer});

        surface.present();
    }

    return 0;
}
