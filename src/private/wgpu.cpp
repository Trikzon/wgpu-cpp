#include "wgpu.hpp"

#include <iostream>

namespace wgpu
{
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
        wgpuInstanceProcessEvents(m_handle);
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

        const WGPURequestAdapterOptions wgpu_options = {
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
        const WGPUSurfaceConfiguration wgpu_configuration = {
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

        return SurfaceCapabilities{
            .next_in_chain = reinterpret_cast<ChainedStructOut *>(wgpu_capabilities.nextInChain),
#ifdef WEBGPU_BACKEND_DAWN
            .usages = static_cast<TextureUsageFlags>(wgpu_capabilities.usages),
#endif
            .formats = {
                reinterpret_cast<const TextureFormat *>(wgpu_capabilities.formats),
                reinterpret_cast<const TextureFormat *>(wgpu_capabilities.formats) + wgpu_capabilities.formatCount
            },
            .present_modes = {
                reinterpret_cast<const PresentMode *>(wgpu_capabilities.presentModes),
                reinterpret_cast<const PresentMode *>(wgpu_capabilities.presentModes) + wgpu_capabilities.presentModeCount
            },
            .alpha_modes = {
                reinterpret_cast<const CompositeAlphaMode *>(wgpu_capabilities.alphaModes),
                reinterpret_cast<const CompositeAlphaMode *>(wgpu_capabilities.alphaModes) + wgpu_capabilities.alphaModeCount
            },
        };
    }

    SurfaceTexture Surface::get_current_texture() const
    {
        WGPUSurfaceTexture wgpu_surface_texture{};
        wgpuSurfaceGetCurrentTexture(m_handle, &wgpu_surface_texture);

        return {
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
        return TextureView{wgpuTextureCreateView(m_handle, nullptr)};
    }

    TextureView Texture::create_view(const TextureViewDescriptor &descriptor) const
    {
        const WGPUTextureViewDescriptor wgpu_descriptor = {
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

        return TextureView{wgpuTextureCreateView(m_handle, &wgpu_descriptor)};
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

    TextureView::TextureView(const WGPUTextureView &handle) : m_handle(handle)
    {

    }

    TextureView::~TextureView()
    {
        if (m_handle != nullptr)
        {
            wgpuTextureViewRelease(m_handle);
        }
    }

    TextureView::TextureView(const TextureView &other) : m_handle(other.m_handle)
    {
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
        std::swap(m_handle, other.m_handle);
    }

    TextureView& TextureView::operator=(const TextureView &other)
    {
        if (this != &other)
        {
            if (m_handle != nullptr)
            {
                wgpuTextureViewRelease(m_handle);
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
