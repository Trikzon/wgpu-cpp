#include "wgpu.hpp"

#include <iostream>

namespace wgpu
{
    ColorWriteMaskFlags operator|(const ColorWriteMaskFlags lhs, const ColorWriteMaskFlags rhs)
    {
        return static_cast<ColorWriteMaskFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }

    ColorWriteMaskFlags & operator|=(ColorWriteMaskFlags &lhs, const ColorWriteMaskFlags rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    ColorWriteMaskFlags operator&(const ColorWriteMaskFlags lhs, const ColorWriteMaskFlags rhs)
    {
        return static_cast<ColorWriteMaskFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }

    ColorWriteMaskFlags & operator&=(ColorWriteMaskFlags& lhs, const ColorWriteMaskFlags rhs)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    bool operator==(const ColorWriteMaskFlags lhs, const ColorWriteMaskFlags rhs)
    {
        return static_cast<uint32_t>(lhs) == static_cast<uint32_t>(rhs);
    }

    ShaderStageFlags operator|(const ShaderStageFlags lhs, const ShaderStageFlags rhs)
    {
        return static_cast<ShaderStageFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }

    ShaderStageFlags & operator|=(ShaderStageFlags &lhs, const ShaderStageFlags rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    ShaderStageFlags operator&(const ShaderStageFlags lhs, const ShaderStageFlags rhs)
    {
        return static_cast<ShaderStageFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }

    ShaderStageFlags & operator&=(ShaderStageFlags& lhs, const ShaderStageFlags rhs)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    bool operator==(const ShaderStageFlags lhs, const ShaderStageFlags rhs)
    {
        return static_cast<uint32_t>(lhs) == static_cast<uint32_t>(rhs);
    }

    TextureUsageFlags operator|(const TextureUsageFlags lhs, const TextureUsageFlags rhs)
    {
        return static_cast<TextureUsageFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }

    TextureUsageFlags & operator|=(TextureUsageFlags &lhs, const TextureUsageFlags rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    TextureUsageFlags operator&(const TextureUsageFlags lhs, const TextureUsageFlags rhs)
    {
        return static_cast<TextureUsageFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }

    TextureUsageFlags & operator&=(TextureUsageFlags& lhs, const TextureUsageFlags rhs)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    bool operator==(const TextureUsageFlags lhs, const TextureUsageFlags rhs)
    {
        return static_cast<uint32_t>(lhs) == static_cast<uint32_t>(rhs);
    }

    Adapter::Adapter(const WGPUAdapter &handle) : m_handle(handle)
    {

    }

    Adapter::~Adapter()
    {
        if (m_handle != nullptr)
        {
            wgpuAdapterRelease(m_handle);
        }
    }

    Adapter::Adapter(const Adapter &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuAdapterReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuAdapterAddRef(m_handle);
#endif
        }
    }

    Adapter::Adapter(Adapter &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Adapter & Adapter::operator=(const Adapter &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuAdapterRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuAdapterReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuAdapterAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    Adapter & Adapter::operator=(Adapter &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUAdapter Adapter::c_ptr() const
    {
        return m_handle;
    }

    std::expected<Device, std::string> Adapter::create_device(const DeviceDescriptor &descriptor) const
    {
        std::expected<Device, std::string> result = std::unexpected("Request did not end.");
        bool request_ended = false;

        auto on_device_request_ended = [&result, &request_ended](const RequestDeviceStatus status,
            const Device& device, const std::string &message)
        {
            if (status == RequestDeviceStatus::Success)
            {
                result = device;
            }
            else
            {
                result = std::unexpected(message);
            }
            request_ended = true;
        };

        auto handle = request_device(descriptor, on_device_request_ended);

        // TODO: emscripten_sleep

        return result;
    }

    std::vector<FeatureName> Adapter::enumerate_features() const
    {
        const auto feature_count = wgpuAdapterEnumerateFeatures(m_handle, nullptr);

        std::vector<FeatureName> features(feature_count);
        wgpuAdapterEnumerateFeatures(m_handle, reinterpret_cast<WGPUFeatureName*>(features.data()));

        return features;
    }

    std::optional<SupportedLimits> Adapter::get_limits() const
    {
        WGPUSupportedLimits wgpu_supported_limits{};
        const auto result = wgpuAdapterGetLimits(m_handle, &wgpu_supported_limits);

#ifdef WEBGPU_BACKEND_WGPU
        if (!result)
#endif
#ifdef WEBGPU_BACKEND_DAWN
        if (result != WGPUStatus_Success)
#endif
        {
            return std::nullopt;
        }

        return *reinterpret_cast<SupportedLimits *>(&wgpu_supported_limits);
    }

    std::optional<AdapterProperties> Adapter::get_properties() const
    {
        WGPUAdapterProperties wgpu_properties{};
#ifdef WEBGPU_BACKEND_WGPU
        wgpuAdapterGetProperties(m_handle, &wgpu_properties);
#endif
#ifdef WEBGPU_BACKEND_DAWN
        if (wgpuAdapterGetProperties(m_handle, &wgpu_properties) != WGPUStatus_Success)
        {
            return std::nullopt;
        }
#endif

        return AdapterProperties{
            .next_in_chain = reinterpret_cast<ChainedStructOut *>(wgpu_properties.nextInChain),
            .vendor_id = wgpu_properties.vendorID,
            .vendor_name = wgpu_properties.vendorName,
            .architecture = wgpu_properties.architecture,
            .device_id = wgpu_properties.deviceID,
            .name = wgpu_properties.name,
            .driver_description = wgpu_properties.driverDescription,
            .adapter_type = static_cast<AdapterType>(wgpu_properties.adapterType),
            .backend_type = static_cast<BackendType>(wgpu_properties.backendType),
        };
    }

    bool Adapter::has_feature(const FeatureName feature) const
    {
        return wgpuAdapterHasFeature(m_handle, static_cast<WGPUFeatureName>(feature));
    }

    std::unique_ptr<RequestDeviceCallback> Adapter::request_device(const DeviceDescriptor &descriptor,
        RequestDeviceCallback &&callback) const
    {
        auto handle = std::make_unique<RequestDeviceCallback>(callback);
        static auto on_request_ended = [](WGPURequestDeviceStatus status, WGPUDevice device,
            const char *message, void *user_data) -> void
        {
            const RequestDeviceCallback& callback = *static_cast<RequestDeviceCallback*>(user_data);
            callback(static_cast<RequestDeviceStatus>(status), Device{device}, message ? message : "");
        };

        // TODO: Allow user to set uncaptured error callback
#ifdef WEBGPU_BACKEND_DAWN
        static auto on_uncaptured_error = [](WGPUErrorType type, const char *message, void *user_data) -> void
        {
            std::cerr << "Uncaptured error: " << message << std::endl;
        };
#endif

        const WGPUDeviceDescriptor wgpu_descriptor = {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .requiredFeatureCount = descriptor.required_features.size(),
            .requiredFeatures = reinterpret_cast<const WGPUFeatureName *>(descriptor.required_features.data()),
            .requiredLimits = reinterpret_cast<const WGPURequiredLimits *>(descriptor.required_limits),
            .defaultQueue = {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.default_queue.next_in_chain),
                .label = descriptor.default_queue.label.c_str()
            },
            .deviceLostCallback = nullptr,
            .deviceLostUserdata = nullptr,
#ifdef WEBGPU_BACKEND_DAWN
            .deviceLostCallbackInfo = {},
            .uncapturedErrorCallbackInfo = {
                .callback = on_uncaptured_error,
            },
#endif
        };

        wgpuAdapterRequestDevice(m_handle, &wgpu_descriptor, on_request_ended, handle.get());
        return handle;
    }

    BindGroupLayout::BindGroupLayout(const WGPUBindGroupLayout &handle) : m_handle(handle)
    {

    }

    BindGroupLayout::~BindGroupLayout()
    {
        if (m_handle != nullptr)
        {
            wgpuBindGroupLayoutRelease(m_handle);
        }
    }

    BindGroupLayout::BindGroupLayout(const BindGroupLayout &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuBindGroupLayoutReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuBindGroupLayoutAddRef(m_handle);
#endif
        }
    }

    BindGroupLayout::BindGroupLayout(BindGroupLayout &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    BindGroupLayout & BindGroupLayout::operator=(const BindGroupLayout &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuBindGroupLayoutRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuBindGroupLayoutReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuBindGroupLayoutAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    BindGroupLayout & BindGroupLayout::operator=(BindGroupLayout &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUBindGroupLayout BindGroupLayout::c_ptr() const
    {
        return m_handle;
    }

    CommandBuffer::CommandBuffer(const WGPUCommandBuffer &handle) : m_handle(handle)
    {

    }

    CommandBuffer::~CommandBuffer()
    {
        if (m_handle != nullptr)
        {
            wgpuCommandBufferRelease(m_handle);
        }
    }

    CommandBuffer::CommandBuffer(const CommandBuffer &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuCommandBufferReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuCommandBufferAddRef(m_handle);
#endif
        }
    }

    CommandBuffer::CommandBuffer(CommandBuffer &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    CommandBuffer & CommandBuffer::operator=(const CommandBuffer &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuCommandBufferRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuCommandBufferReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuCommandBufferAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    CommandBuffer & CommandBuffer::operator=(CommandBuffer &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUCommandBuffer CommandBuffer::c_ptr() const
    {
        return m_handle;
    }

    CommandEncoder::CommandEncoder(const WGPUCommandEncoder &handle) : m_handle(handle)
    {

    }

    CommandEncoder::~CommandEncoder()
    {
        if (m_handle != nullptr)
        {
            wgpuCommandEncoderRelease(m_handle);
        }
    }

    CommandEncoder::CommandEncoder(const CommandEncoder &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuCommandEncoderReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuCommandEncoderAddRef(m_handle);
#endif
        }
    }

    CommandEncoder::CommandEncoder(CommandEncoder &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    CommandEncoder & CommandEncoder::operator=(const CommandEncoder &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuCommandEncoderRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuCommandEncoderReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuCommandEncoderAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    CommandEncoder & CommandEncoder::operator=(CommandEncoder &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUCommandEncoder CommandEncoder::c_ptr() const
    {
        return m_handle;
    }

    RenderPassEncoder CommandEncoder::begin_render_pass(const RenderPassDescriptor &descriptor) const
    {
        std::vector<WGPURenderPassColorAttachment> wgpu_color_attachments;
        wgpu_color_attachments.reserve(descriptor.color_attachments.size());
        for (const auto &color_attachment : descriptor.color_attachments)
        {
            wgpu_color_attachments.push_back(
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(color_attachment.next_in_chain),
                .view = color_attachment.view ? color_attachment.view->c_ptr() : nullptr,
#ifdef WEBGPU_BACKEND_DAWN
                .depthSlice = color_attachment.depth_slice,
#endif
                .resolveTarget = color_attachment.resolve_target ? color_attachment.resolve_target->c_ptr() : nullptr,
                .loadOp = static_cast<WGPULoadOp>(color_attachment.load_op),
                .storeOp = static_cast<WGPUStoreOp>(color_attachment.store_op),
                .clearValue = *reinterpret_cast<const WGPUColor*>(&color_attachment.clear_value),
            });
        }

        WGPURenderPassDepthStencilAttachment wgpu_depth_stencil_attachment{};
        if (descriptor.depth_stencil_attachment)
        {
            wgpu_depth_stencil_attachment = WGPURenderPassDepthStencilAttachment
            {
                .view = descriptor.depth_stencil_attachment->view.c_ptr(),
                .depthLoadOp = static_cast<WGPULoadOp>(descriptor.depth_stencil_attachment->depth_load_op),
                .depthStoreOp = static_cast<WGPUStoreOp>(descriptor.depth_stencil_attachment->depth_store_op),
                .depthClearValue = descriptor.depth_stencil_attachment->depth_clear_value,
                .stencilLoadOp = static_cast<WGPULoadOp>(descriptor.depth_stencil_attachment->stencil_load_op),
                .stencilStoreOp = static_cast<WGPUStoreOp>(descriptor.depth_stencil_attachment->stencil_store_op),
                .stencilClearValue = descriptor.depth_stencil_attachment->stencil_clear_value,
                .stencilReadOnly = descriptor.depth_stencil_attachment->stencil_read_only,
            };
        }

        const WGPURenderPassDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .colorAttachmentCount = wgpu_color_attachments.size(),
            .colorAttachments = wgpu_color_attachments.data(),
            .depthStencilAttachment = descriptor.depth_stencil_attachment ? &wgpu_depth_stencil_attachment : nullptr,
            .occlusionQuerySet = nullptr,
            .timestampWrites = nullptr,
        };

        return RenderPassEncoder{wgpuCommandEncoderBeginRenderPass(m_handle, &wgpu_descriptor)};
    }

    CommandBuffer CommandEncoder::finish(const CommandBufferDescriptor &descriptor) const
    {
        const WGPUCommandBufferDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
        };

        return CommandBuffer{wgpuCommandEncoderFinish(m_handle, &wgpu_descriptor)};
    }

    Device::Device(const WGPUDevice &handle) : m_handle(handle)
    {

    }

    Device::~Device()
    {
        if (m_handle != nullptr)
        {
            wgpuDeviceRelease(m_handle);
        }
    }

    Device::Device(const Device &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuDeviceReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuDeviceAddRef(m_handle);
#endif
        }
    }

    Device::Device(Device &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Device & Device::operator=(const Device &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuDeviceRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuDeviceReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuDeviceAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    Device & Device::operator=(Device &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUDevice Device::c_ptr() const
    {
        return m_handle;
    }

    BindGroupLayout Device::create_bind_group_layout(const BindGroupLayoutDescriptor &descriptor) const
    {
        std::vector<WGPUBindGroupLayoutEntry> wgpu_entries;
        wgpu_entries.reserve(descriptor.entries.size());
        for (const BindGroupLayoutEntry &entry : descriptor.entries)
        {
            wgpu_entries.push_back(WGPUBindGroupLayoutEntry
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(entry.next_in_chain),
                .binding = entry.binding,
                .visibility = static_cast<WGPUShaderStageFlags>(entry.visibility),
                .buffer = WGPUBufferBindingLayout
                {
                    .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(entry.buffer.next_in_chain),
                    .type = static_cast<WGPUBufferBindingType>(entry.buffer.type),
                    .hasDynamicOffset = entry.buffer.has_dynamic_offset,
                    .minBindingSize = entry.buffer.min_binding_size,
                },
                .sampler = WGPUSamplerBindingLayout
                {
                    .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(entry.sampler.next_in_chain),
                    .type = static_cast<WGPUSamplerBindingType>(entry.sampler.type),
                },
                .texture = WGPUTextureBindingLayout
                {
                    .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(entry.texture.next_in_chain),
                    .sampleType = static_cast<WGPUTextureSampleType>(entry.texture.sample_type),
                    .viewDimension = static_cast<WGPUTextureViewDimension>(entry.texture.view_dimension),
                    .multisampled = entry.texture.multisampled,
                },
                .storageTexture = WGPUStorageTextureBindingLayout
                {
                    .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(entry.storage_texture.next_in_chain),
                    .access = static_cast<WGPUStorageTextureAccess>(entry.storage_texture.access),
                    .format = static_cast<WGPUTextureFormat>(entry.storage_texture.format),
                    .viewDimension = static_cast<WGPUTextureViewDimension>(entry.storage_texture.view_dimension),
                },
            });
        }

        const WGPUBindGroupLayoutDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .entryCount = wgpu_entries.size(),
            .entries = wgpu_entries.data(),
        };

        return BindGroupLayout{wgpuDeviceCreateBindGroupLayout(m_handle, &wgpu_descriptor)};
    }

    CommandEncoder Device::create_command_encoder(const CommandEncoderDescriptor &descriptor) const
    {
        const WGPUCommandEncoderDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
        };

        return CommandEncoder{wgpuDeviceCreateCommandEncoder(m_handle, &wgpu_descriptor)};
    }

    PipelineLayout Device::create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) const
    {
        std::vector<WGPUBindGroupLayout> wgpu_bind_group_layouts;
        wgpu_bind_group_layouts.reserve(descriptor.bind_group_layouts.size());
        for (const auto &bind_group_layout : descriptor.bind_group_layouts)
        {
            wgpu_bind_group_layouts.push_back(bind_group_layout.c_ptr());
        }

        const WGPUPipelineLayoutDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .bindGroupLayoutCount = wgpu_bind_group_layouts.size(),
            .bindGroupLayouts = wgpu_bind_group_layouts.data(),
        };

        return PipelineLayout{wgpuDeviceCreatePipelineLayout(m_handle, &wgpu_descriptor)};
    }

    RenderPipeline Device::create_render_pipeline(const RenderPipelineDescriptor &descriptor) const
    {
        std::vector<WGPUConstantEntry> wgpu_vertex_constants;
        wgpu_vertex_constants.reserve(descriptor.vertex.constants.size());
        for (const auto &constant : descriptor.vertex.constants)
        {
            wgpu_vertex_constants.push_back({
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(constant.next_in_chain),
                .key = constant.key.c_str(),
                .value = constant.value,
            });
        }

        std::vector<WGPUVertexBufferLayout> wgpu_vertex_buffers;
        wgpu_vertex_buffers.reserve(descriptor.vertex.buffers.size());
        std::vector<std::vector<WGPUVertexAttribute>> wgpu_vertex_buffers_attributes(descriptor.vertex.buffers.size());
        for (auto i = 0; i < descriptor.vertex.buffers.size(); ++i)
        {
            const auto &buffer = descriptor.vertex.buffers[i];
            auto &wgpu_vertex_buffer_attributes = wgpu_vertex_buffers_attributes[i];

            wgpu_vertex_buffer_attributes.reserve(buffer.attributes.size());
            for (const auto &attribute : buffer.attributes)
            {
                wgpu_vertex_buffer_attributes.push_back(
                {
                    .format = static_cast<WGPUVertexFormat>(attribute.format),
                    .offset = attribute.offset,
                    .shaderLocation = attribute.shader_location,
                });
            }

            wgpu_vertex_buffers.push_back(
            {
                .arrayStride = buffer.array_stride,
                .stepMode = static_cast<WGPUVertexStepMode>(buffer.step_mode),
                .attributeCount = wgpu_vertex_buffer_attributes.size(),
                .attributes = wgpu_vertex_buffer_attributes.data(),
            });
        }

        std::optional<WGPUDepthStencilState> wgpu_depth_stencil;
        if (descriptor.depth_stencil)
        {
            wgpu_depth_stencil = WGPUDepthStencilState
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.depth_stencil->next_in_chain),
                .format = static_cast<WGPUTextureFormat>(descriptor.depth_stencil->format),
                .depthWriteEnabled = descriptor.depth_stencil->depth_write_enabled,
                .depthCompare = static_cast<WGPUCompareFunction>(descriptor.depth_stencil->depth_compare),
                .stencilFront = WGPUStencilFaceState
                {
                    .compare = static_cast<WGPUCompareFunction>(descriptor.depth_stencil->stencil_front.compare),
                    .failOp = static_cast<WGPUStencilOperation>(descriptor.depth_stencil->stencil_front.fail_op),
                    .depthFailOp = static_cast<WGPUStencilOperation>(descriptor.depth_stencil->stencil_front.depth_fail_op),
                    .passOp = static_cast<WGPUStencilOperation>(descriptor.depth_stencil->stencil_front.pass_op),
                },
                .stencilBack = WGPUStencilFaceState
                {
                    .compare = static_cast<WGPUCompareFunction>(descriptor.depth_stencil->stencil_back.compare),
                    .failOp = static_cast<WGPUStencilOperation>(descriptor.depth_stencil->stencil_back.fail_op),
                    .depthFailOp = static_cast<WGPUStencilOperation>(descriptor.depth_stencil->stencil_back.depth_fail_op),
                    .passOp = static_cast<WGPUStencilOperation>(descriptor.depth_stencil->stencil_back.pass_op),
                },
                .stencilReadMask = descriptor.depth_stencil->stencil_read_mask,
                .stencilWriteMask = descriptor.depth_stencil->stencil_write_mask,
                .depthBias = descriptor.depth_stencil->depth_bias,
                .depthBiasSlopeScale = descriptor.depth_stencil->depth_bias_slope_scale,
                .depthBiasClamp = descriptor.depth_stencil->depth_bias_clamp,
            };
        }

        // We need to keep all of these in scope until the end of the function.
        std::optional<WGPUFragmentState> wgpu_fragment;
        std::vector<WGPUConstantEntry> wgpu_fragment_constants;
        std::vector<WGPUColorTargetState> wgpu_fragment_targets;
        std::optional<WGPUBlendState> wgpu_fragment_target_blend;
        if (descriptor.fragment)
        {
            wgpu_fragment_constants.reserve(descriptor.fragment->constants.size());
            for (const auto &constant : descriptor.fragment->constants)
            {
                wgpu_fragment_constants.push_back(
                {
                    .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(constant.next_in_chain),
                    .key = constant.key.c_str(),
                    .value = constant.value,
                });
            }

            wgpu_fragment_targets.reserve(descriptor.fragment->targets.size());
            for (const auto &target : descriptor.fragment->targets)
            {
                if (target.blend)
                {
                    wgpu_fragment_target_blend = WGPUBlendState
                    {
                        .color = WGPUBlendComponent
                        {
                            .operation = static_cast<WGPUBlendOperation>(target.blend->color.operation),
                            .srcFactor = static_cast<WGPUBlendFactor>(target.blend->color.src_factor),
                            .dstFactor = static_cast<WGPUBlendFactor>(target.blend->color.dst_factor),
                        },
                        .alpha = WGPUBlendComponent
                        {
                            .operation = static_cast<WGPUBlendOperation>(target.blend->alpha.operation),
                            .srcFactor = static_cast<WGPUBlendFactor>(target.blend->alpha.src_factor),
                            .dstFactor = static_cast<WGPUBlendFactor>(target.blend->alpha.dst_factor),
                        }
                    };
                }

                wgpu_fragment_targets.push_back(
                {
                    .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(target.next_in_chain),
                    .format = static_cast<WGPUTextureFormat>(target.format),
                    .blend = wgpu_fragment_target_blend ? &wgpu_fragment_target_blend.value() : nullptr,
                    .writeMask = static_cast<WGPUColorWriteMaskFlags>(target.write_mask),
                });
            }

            wgpu_fragment = WGPUFragmentState
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.fragment->next_in_chain),
                .module = descriptor.fragment->module.c_ptr(),
                .entryPoint = descriptor.fragment->entry_point ? descriptor.fragment->entry_point->c_str() : nullptr,
                .constantCount = wgpu_fragment_constants.size(),
                .constants = wgpu_fragment_constants.data(),
                .targetCount = wgpu_fragment_targets.size(),
                .targets = wgpu_fragment_targets.data(),
            };
        }

        const WGPURenderPipelineDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .layout = descriptor.layout ? descriptor.layout->c_ptr() : nullptr,
            .vertex = WGPUVertexState
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.vertex.next_in_chain),
                .module = descriptor.vertex.module.c_ptr(),
                .entryPoint = descriptor.vertex.entry_point ? descriptor.vertex.entry_point->c_str() : nullptr,
                .constantCount = wgpu_vertex_constants.size(),
                .constants = wgpu_vertex_constants.data(),
                .bufferCount = wgpu_vertex_buffers.size(),
                .buffers = wgpu_vertex_buffers.data(),
            },
            .primitive = WGPUPrimitiveState
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.primitive.next_in_chain),
                .topology = static_cast<WGPUPrimitiveTopology>(descriptor.primitive.topology),
                .stripIndexFormat = static_cast<WGPUIndexFormat>(descriptor.primitive.strip_index_format),
                .frontFace = static_cast<WGPUFrontFace>(descriptor.primitive.front_face),
                .cullMode = static_cast<WGPUCullMode>(descriptor.primitive.cull_mode),
            },
            .depthStencil = wgpu_depth_stencil ? &wgpu_depth_stencil.value() : nullptr,
            .multisample = WGPUMultisampleState
            {
                .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.multisample.next_in_chain),
                .count = descriptor.multisample.count,
                .mask = descriptor.multisample.mask,
                .alphaToCoverageEnabled = descriptor.multisample.alpha_to_coverage_enabled,
            },
            .fragment = wgpu_fragment ? &wgpu_fragment.value() : nullptr,
        };

        return RenderPipeline{wgpuDeviceCreateRenderPipeline(m_handle, &wgpu_descriptor)};
    }

    ShaderModule Device::create_shader_module(const ShaderModuleDescriptor &descriptor) const
    {
        const WGPUShaderModuleDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
#ifdef WEBGPU_BACKEND_WGPU
            .hintCount = 0,
            .hints = nullptr,
#endif
        };

        return ShaderModule{wgpuDeviceCreateShaderModule(m_handle, &wgpu_descriptor)};
    }

    Texture Device::create_texture(const TextureDescriptor &descriptor) const
    {
        const WGPUTextureDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .usage = static_cast<WGPUTextureUsageFlags>(descriptor.usage),
            .dimension = static_cast<WGPUTextureDimension>(descriptor.dimension),
            .size = WGPUExtent3D
            {
                .width = descriptor.size.width,
                .height = descriptor.size.height,
                .depthOrArrayLayers = descriptor.size.depth_or_array_layers,
            },
            .format = static_cast<WGPUTextureFormat>(descriptor.format),
            .mipLevelCount = descriptor.mip_level_count,
            .sampleCount = descriptor.sample_count,
            .viewFormatCount = descriptor.view_formats.size(),
            .viewFormats = reinterpret_cast<const WGPUTextureFormat *>(descriptor.view_formats.data()),
        };

        return Texture{wgpuDeviceCreateTexture(m_handle, &wgpu_descriptor)};
    }

    Queue Device::get_queue() const
    {
        return Queue{wgpuDeviceGetQueue(m_handle)};
    }

    Instance::Instance(const WGPUInstance &handle) : m_handle(handle)
    {

    }

    Instance::~Instance()
    {
        if (m_handle != nullptr)
        {
            wgpuInstanceRelease(m_handle);
        }
    }

    Instance::Instance(const Instance &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuInstanceReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuInstanceAddRef(m_handle);
#endif
        }
    }

    Instance::Instance(Instance &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Instance & Instance::operator=(const Instance &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuInstanceRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuInstanceReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuInstanceAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    Instance & Instance::operator=(Instance &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUInstance Instance::c_ptr() const
    {
        return m_handle;
    }

    std::expected<Adapter, std::string> Instance::create_adapter(const RequestAdapterOptions &options) const
    {
        std::expected<Adapter, std::string> result = std::unexpected("Request did not end.");
        bool request_ended = false;

        auto on_adapter_request_ended = [&result, &request_ended](const RequestAdapterStatus status,
            const Adapter& adapter, const std::string &message)
        {
            if (status == RequestAdapterStatus::Success)
            {
                result = adapter;
            }
            else
            {
                result = std::unexpected(message);
            }
            request_ended = true;
        };

        auto handle = request_adapter(options, on_adapter_request_ended);

        // TODO: emscripten_sleep

        return result;
    }

    void Instance::process_events() const
    {
        // wgpuInstanceProcessEvents exists on WGPU, but panics if called.
#ifdef WEBGPU_BACKEND_DAWN
        wgpuInstanceProcessEvents(m_handle);
#endif
    }

    std::unique_ptr<RequestAdapterCallback> Instance::request_adapter(const RequestAdapterOptions &options,
        RequestAdapterCallback&& callback) const
    {
        auto handle = std::make_unique<RequestAdapterCallback>(callback);
        static auto on_request_ended = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
            const char *message, void *user_data) -> void
        {
            const RequestAdapterCallback& callback = *static_cast<RequestAdapterCallback*>(user_data);
            callback(static_cast<RequestAdapterStatus>(status), Adapter{adapter}, message ? message : "");
        };

        const WGPURequestAdapterOptions wgpu_options
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(options.next_in_chain),
            .compatibleSurface = options.compatible_surface ? options.compatible_surface->c_ptr() : nullptr,
            .powerPreference = static_cast<WGPUPowerPreference>(options.power_preference),
            .backendType = static_cast<WGPUBackendType>(options.backend_type),
            .forceFallbackAdapter = options.force_fallback_adapter,
#ifdef WEBGPU_BACKEND_DAWN
            .compatibilityMode = options.compatibility_mode,
#endif
        };

        wgpuInstanceRequestAdapter(m_handle, &wgpu_options, on_request_ended, handle.get());
        return handle;
    }

    PipelineLayout::PipelineLayout(const WGPUPipelineLayout &handle) : m_handle(handle)
    {

    }

    PipelineLayout::~PipelineLayout()
    {
        if (m_handle != nullptr)
        {
            wgpuPipelineLayoutRelease(m_handle);
        }
    }

    PipelineLayout::PipelineLayout(const PipelineLayout &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuPipelineLayoutReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuPipelineLayoutAddRef(m_handle);
#endif
        }
    }

    PipelineLayout::PipelineLayout(PipelineLayout &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    PipelineLayout& PipelineLayout::operator=(const PipelineLayout &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuPipelineLayoutRelease(m_handle);
            }

            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuPipelineLayoutReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuPipelineLayoutAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    PipelineLayout& PipelineLayout::operator=(PipelineLayout &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUPipelineLayout PipelineLayout::c_ptr() const
    {
        return m_handle;
    }

    Queue::Queue(const WGPUQueue &handle) : m_handle(handle)
    {

    }

    Queue::~Queue()
    {
        if (m_handle != nullptr)
        {
            wgpuQueueRelease(m_handle);
        }
    }

    Queue::Queue(const Queue &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuQueueReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuQueueAddRef(m_handle);
#endif
        }
    }

    Queue::Queue(Queue &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Queue& Queue::operator=(const Queue &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuQueueRelease(m_handle);
            }

            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuQueueReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuQueueAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    Queue& Queue::operator=(Queue &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUQueue Queue::c_ptr() const
    {
        return m_handle;
    }

    void Queue::submit(const std::vector<CommandBuffer> &commands) const
    {
        std::vector<WGPUCommandBuffer> wgpu_commands;
        wgpu_commands.reserve(commands.size());
        for (const auto &command : commands)
        {
            wgpu_commands.push_back(command.c_ptr());
        }

        wgpuQueueSubmit(m_handle, wgpu_commands.size(), wgpu_commands.data());
    }

    RenderPassEncoder::RenderPassEncoder(const WGPURenderPassEncoder &handle) : m_handle(handle)
    {

    }

    RenderPassEncoder::~RenderPassEncoder()
    {
        if (m_handle != nullptr)
        {
            wgpuRenderPassEncoderRelease(m_handle);
        }
    }

    RenderPassEncoder::RenderPassEncoder(const RenderPassEncoder &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuRenderPassEncoderReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuRenderPassEncoderAddRef(m_handle);
#endif
        }
    }

    RenderPassEncoder::RenderPassEncoder(RenderPassEncoder &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    RenderPassEncoder& RenderPassEncoder::operator=(const RenderPassEncoder &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuRenderPassEncoderRelease(m_handle);
            }

            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuRenderPassEncoderReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuRenderPassEncoderAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    RenderPassEncoder& RenderPassEncoder::operator=(RenderPassEncoder &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPURenderPassEncoder RenderPassEncoder::c_ptr() const
    {
        return m_handle;
    }

    void RenderPassEncoder::draw(const uint32_t vertex_count, const uint32_t instance_count,
        const uint32_t first_vertex, const uint32_t first_instance) const
    {
        wgpuRenderPassEncoderDraw(m_handle, vertex_count, instance_count, first_vertex, first_instance);
    }

    void RenderPassEncoder::end() const
    {
        wgpuRenderPassEncoderEnd(m_handle);
    }

    void RenderPassEncoder::set_pipeline(const RenderPipeline &pipeline) const
    {
        wgpuRenderPassEncoderSetPipeline(m_handle, pipeline.c_ptr());
    }

    RenderPipeline::RenderPipeline(const WGPURenderPipeline &handle) : m_handle(handle)
    {

    }

    RenderPipeline::~RenderPipeline()
    {
        if (m_handle != nullptr)
        {
            wgpuRenderPipelineRelease(m_handle);
        }
    }

    RenderPipeline::RenderPipeline(const RenderPipeline &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuRenderPipelineReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuRenderPipelineAddRef(m_handle);
#endif
        }
    }

    RenderPipeline::RenderPipeline(RenderPipeline &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    RenderPipeline& RenderPipeline::operator=(const RenderPipeline &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuRenderPipelineRelease(m_handle);
            }

            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuRenderPipelineReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuRenderPipelineAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    RenderPipeline& RenderPipeline::operator=(RenderPipeline &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPURenderPipeline RenderPipeline::c_ptr() const
    {
        return m_handle;
    }

    ShaderModule::ShaderModule(const WGPUShaderModule &handle) : m_handle(handle)
    {

    }

    ShaderModule::~ShaderModule()
    {
        if (m_handle != nullptr)
        {
            wgpuShaderModuleRelease(m_handle);
        }
    }

    ShaderModule::ShaderModule(const ShaderModule &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuShaderModuleReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuShaderModuleAddRef(m_handle);
#endif
        }
    }

    ShaderModule::ShaderModule(ShaderModule &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    ShaderModule& ShaderModule::operator=(const ShaderModule &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuShaderModuleRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuShaderModuleReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuShaderModuleAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    ShaderModule& ShaderModule::operator=(ShaderModule &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUShaderModule ShaderModule::c_ptr() const
    {
        return m_handle;
    }

    Surface::Surface(const WGPUSurface &handle) : m_handle(handle)
    {

    }

    Surface::~Surface()
    {
        if (m_handle != nullptr)
        {
            wgpuSurfaceRelease(m_handle);
        }
    }

    Surface::Surface(const Surface &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuSurfaceReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuSurfaceAddRef(m_handle);
#endif
        }
    }

    Surface::Surface(Surface &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Surface& Surface::operator=(const Surface &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuSurfaceRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuSurfaceReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuSurfaceAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    Surface& Surface::operator=(Surface &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUSurface Surface::c_ptr() const
    {
        return m_handle;
    }

    void Surface::configure(const SurfaceConfiguration &configuration) const
    {
        const WGPUSurfaceConfiguration wgpu_configuration
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(configuration.next_in_chain),
            .device = configuration.device.c_ptr(),
            .format = static_cast<WGPUTextureFormat>(configuration.format),
            .usage = static_cast<WGPUTextureUsageFlags>(configuration.usage),
            .viewFormatCount = configuration.view_formats.size(),
            .viewFormats = reinterpret_cast<const WGPUTextureFormat *>(configuration.view_formats.data()),
            .alphaMode = static_cast<WGPUCompositeAlphaMode>(configuration.alpha_mode),
            .width = configuration.width,
            .height = configuration.height,
            .presentMode = static_cast<WGPUPresentMode>(configuration.present_mode),
        };

        wgpuSurfaceConfigure(m_handle, &wgpu_configuration);
    }

    SurfaceCapabilities Surface::get_capabilities(const Adapter &adapter) const
    {
        WGPUSurfaceCapabilities wgpu_capabilities{};
        // TODO: On Dawn, this returns a WGPUStatus. This should be checked and handled.
        // However, it only returns WGPUStatus_Error for some reason during testing.
        wgpuSurfaceGetCapabilities(m_handle, adapter.c_ptr(), &wgpu_capabilities);

        return SurfaceCapabilities
        {
            .next_in_chain = reinterpret_cast<ChainedStructOut *>(wgpu_capabilities.nextInChain),
#ifdef WEBGPU_BACKEND_DAWN
            .usages = static_cast<TextureUsageFlags>(wgpu_capabilities.usages),
#endif
            .formats = std::vector<TextureFormat>
            {
                reinterpret_cast<const TextureFormat *>(wgpu_capabilities.formats),
                reinterpret_cast<const TextureFormat *>(wgpu_capabilities.formats) + wgpu_capabilities.formatCount
            },
            .present_modes = std::vector<PresentMode>
            {
                reinterpret_cast<const PresentMode *>(wgpu_capabilities.presentModes),
                reinterpret_cast<const PresentMode *>(wgpu_capabilities.presentModes) + wgpu_capabilities.presentModeCount
            },
            .alpha_modes = std::vector<CompositeAlphaMode>
            {
                reinterpret_cast<const CompositeAlphaMode *>(wgpu_capabilities.alphaModes),
                reinterpret_cast<const CompositeAlphaMode *>(wgpu_capabilities.alphaModes) + wgpu_capabilities.alphaModeCount
            },
        };
    }

    SurfaceTexture Surface::get_current_texture() const
    {
        WGPUSurfaceTexture wgpu_surface_texture{};
        wgpuSurfaceGetCurrentTexture(m_handle, &wgpu_surface_texture);

        return SurfaceTexture
        {
            .texture = Texture{wgpu_surface_texture.texture},
            .suboptimal = static_cast<bool>(wgpu_surface_texture.suboptimal),
            .status = static_cast<SurfaceGetCurrentTextureStatus>(wgpu_surface_texture.status),
        };
    }

    void Surface::present() const
    {
        wgpuSurfacePresent(m_handle);
    }

    void Surface::unconfigure() const
    {
        wgpuSurfaceUnconfigure(m_handle);
    }

    Texture::Texture(const WGPUTexture &handle) : m_handle(handle)
    {

    }

    Texture::~Texture()
    {
        if (m_handle != nullptr)
        {
            wgpuTextureRelease(m_handle);
        }
    }

    Texture::Texture(const Texture &other) : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuTextureReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuTextureAddRef(m_handle);
#endif
        }
    }

    Texture::Texture(Texture &&other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Texture & Texture::operator=(const Texture &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuTextureRelease(m_handle);
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
                wgpuTextureReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
                wgpuTextureAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    Texture & Texture::operator=(Texture &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUTexture Texture::c_ptr() const
    {
        return m_handle;
    }

    TextureView Texture::create_view() const
    {
        return TextureView{m_handle, wgpuTextureCreateView(m_handle, nullptr)};
    }

    TextureView Texture::create_view(const TextureViewDescriptor &descriptor) const
    {
        const WGPUTextureViewDescriptor wgpu_descriptor
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(descriptor.next_in_chain),
            .label = descriptor.label.c_str(),
            .format = static_cast<WGPUTextureFormat>(descriptor.format),
            .dimension = static_cast<WGPUTextureViewDimension>(descriptor.dimension),
            .baseMipLevel = descriptor.base_mip_level,
            .mipLevelCount = descriptor.mip_level_count,
            .baseArrayLayer = descriptor.base_array_layer,
            .arrayLayerCount = descriptor.array_layer_count,
            .aspect = static_cast<WGPUTextureAspect>(descriptor.aspect),
        };

        return TextureView{m_handle, wgpuTextureCreateView(m_handle, &wgpu_descriptor)};
    }

    uint32_t Texture::get_depth_or_array_layers() const
    {
        return wgpuTextureGetDepthOrArrayLayers(m_handle);
    }

    TextureDimension Texture::get_dimension() const
    {
        return static_cast<TextureDimension>(wgpuTextureGetDimension(m_handle));
    }

    TextureFormat Texture::get_format() const
    {
        return static_cast<TextureFormat>(wgpuTextureGetFormat(m_handle));
    }

    uint32_t Texture::get_height() const
    {
        return wgpuTextureGetHeight(m_handle);
    }

    uint32_t Texture::get_mip_level_count() const
    {
        return wgpuTextureGetMipLevelCount(m_handle);
    }

    uint32_t Texture::get_sample_count() const
    {
        return wgpuTextureGetSampleCount(m_handle);
    }

    TextureUsageFlags Texture::get_usage() const
    {
        return static_cast<TextureUsageFlags>(wgpuTextureGetUsage(m_handle));
    }

    uint32_t Texture::get_width() const
    {
        return wgpuTextureGetWidth(m_handle);
    }

    TextureView::TextureView(const WGPUTexture &texture, const WGPUTextureView &handle)
        : m_texture(texture), m_handle(handle)
    {
        if (m_texture != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuTextureReference(m_texture);
#elif WEBGPU_BACKEND_DAWN
            wgpuTextureAddRef(m_texture);
#endif
        }
    }

    TextureView::~TextureView()
    {
        if (m_texture != nullptr)
        {
            wgpuTextureRelease(m_texture);
        }

        if (m_handle != nullptr)
        {
            wgpuTextureViewRelease(m_handle);
        }
    }

    TextureView::TextureView(const TextureView &other) : m_texture(other.m_texture), m_handle(other.m_handle)
    {
        if (m_texture != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuTextureReference(m_texture);
#elif WEBGPU_BACKEND_DAWN
            wgpuTextureAddRef(m_texture);
#endif
        }

        if (m_handle != nullptr)
        {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuTextureViewReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuTextureViewAddRef(m_handle);
#endif
        }
    }

    TextureView::TextureView(TextureView &&other) noexcept
    {
        std::swap(m_texture, other.m_texture);
        std::swap(m_handle, other.m_handle);
    }

    TextureView& TextureView::operator=(const TextureView &other)
    {
        if (this != &other)
        {
            if (m_texture != nullptr)
            {
                wgpuTextureRelease(m_texture);
            }

            if (m_handle != nullptr)
            {
                wgpuTextureViewRelease(m_handle);
            }

            m_texture = other.m_texture;
            if (m_texture != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuTextureReference(m_texture);
#elif WEBGPU_BACKEND_DAWN
            wgpuTextureAddRef(m_texture);
#endif
            }

            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
#ifdef WEBGPU_BACKEND_WGPU
            wgpuTextureViewReference(m_handle);
#elif WEBGPU_BACKEND_DAWN
            wgpuTextureViewAddRef(m_handle);
#endif
            }
        }
        return *this;
    }

    TextureView& TextureView::operator=(TextureView &&other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_texture, other.m_texture);
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    WGPUTextureView TextureView::c_ptr() const
    {
        return m_handle;
    }

    Instance create_instance(const InstanceDescriptor &descriptor)
    {
        return Instance{wgpuCreateInstance(reinterpret_cast<const WGPUInstanceDescriptor *>(&descriptor))};
    }
}
