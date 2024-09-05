#include "wgpu.hpp"

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

    Adapter::Adapter(const WGPUAdapter& handle) : m_handle(handle)
    {

    }

    Adapter::~Adapter()
    {
        if (m_handle != nullptr)
        {
            wgpuAdapterRelease(m_handle);
        }
    }

    Adapter::Adapter(const Adapter& other) : m_handle(other.m_handle)
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

    Adapter::Adapter(Adapter&& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Adapter& Adapter::operator=(const Adapter& other)
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

    Adapter& Adapter::operator=(Adapter&& other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
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

#if WEBGPU_BACKEND_WGPU
        if (!result)
#endif
#if WEBGPU_BACKEND_DAWN
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
        const auto result = wgpuAdapterGetProperties(m_handle, &wgpu_properties);
        if (result != WGPUStatus_Success)
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

    WGPUAdapter Adapter::c_ptr() const
    {
        return m_handle;
    }

    Instance::Instance(const WGPUInstance& handle) : m_handle(handle)
    {

    }

    Instance::~Instance()
    {
        if (m_handle != nullptr)
        {
            wgpuInstanceRelease(m_handle);
        }
    }

    Instance::Instance(const Instance& other) : m_handle(other.m_handle)
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

    Instance::Instance(Instance&& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Instance& Instance::operator=(const Instance& other)
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

    Instance& Instance::operator=(Instance&& other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    void Instance::process_events() const
    {
        wgpuInstanceProcessEvents(m_handle);
    }

    std::expected<Adapter, const char *> Instance::create_adapter(const RequestAdapterOptions &options) const
    {
        std::expected<Adapter, const char *> result = std::unexpected("Request did not end.");
        bool request_ended = false;

        auto on_adapter_request_ended = [&result, &request_ended](const RequestAdapterStatus status,
            const Adapter& adapter, const char *message)
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


    std::unique_ptr<RequestAdapterCallback> Instance::request_adapter(const RequestAdapterOptions &options,
        RequestAdapterCallback&& callback) const
    {
        auto handle = std::make_unique<RequestAdapterCallback>(callback);
        static auto on_request_ended = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
            const char *message, void *user_data) -> void
        {
            const RequestAdapterCallback& callback = *static_cast<RequestAdapterCallback*>(user_data);
            callback(static_cast<RequestAdapterStatus>(status), Adapter{adapter}, message);
        };

        const WGPURequestAdapterOptions wgpu_options = {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(options.next_in_chain),
            .compatibleSurface = options.compatible_surface ? options.compatible_surface->c_ptr() : nullptr,
            .powerPreference = static_cast<WGPUPowerPreference>(options.power_preference),
            .backendType = static_cast<WGPUBackendType>(options.backend_type),
            .forceFallbackAdapter = options.force_fallback_adapter,
#if WEBGPU_BACKEND_DAWN
            .compatibilityMode = options.compatibility_mode,
#endif
        };

        wgpuInstanceRequestAdapter(m_handle, &wgpu_options, on_request_ended, handle.get());
        return handle;
    }

    WGPUInstance Instance::c_ptr() const
    {
        return m_handle;
    }

    Surface::Surface(const WGPUSurface& handle) : m_handle(handle)
    {

    }

    Surface::~Surface()
    {
        if (m_handle != nullptr)
        {
            wgpuSurfaceRelease(m_handle);
        }
    }

    Surface::Surface(const Surface& other) : m_handle(other.m_handle)
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

    Surface::Surface(Surface&& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    Surface& Surface::operator=(const Surface& other)
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

    Surface& Surface::operator=(Surface&& other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_handle, other.m_handle);
        }
        return *this;
    }

    void Surface::configure(const SurfaceConfiguration &configuration) const
    {
        const WGPUSurfaceConfiguration wgpu_configuration = {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(configuration.next_in_chain),
            .device = configuration.device,
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

    WGPUSurface Surface::c_ptr() const
    {
        return m_handle;
    }

    Instance create_instance(const InstanceDescriptor &descriptor)
    {
        return Instance{wgpuCreateInstance(reinterpret_cast<const WGPUInstanceDescriptor *>(&descriptor))};
    }
}
