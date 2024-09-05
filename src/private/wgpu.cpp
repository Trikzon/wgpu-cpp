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


    const WGPUChainedStruct * ChainedStruct::c_struct() const
    {
        return reinterpret_cast<const WGPUChainedStruct *>(this);
    }

#if WEBGPU_BACKEND_DAWN
    const WGPUInstanceFeatures * InstanceFeatures::c_struct() const
    {
        return reinterpret_cast<const WGPUInstanceFeatures *>(this);
    }
#endif

    const WGPUInstanceDescriptor * InstanceDescriptor::c_struct() const
    {
        return reinterpret_cast<const WGPUInstanceDescriptor *>(this);
    }

    WGPURequestAdapterOptions RequestAdapterOptions::c_struct() const
    {
        return {
            .nextInChain = next_in_chain->c_struct(),
            .compatibleSurface = compatible_surface ? compatible_surface->c_ptr() : nullptr,
            .powerPreference = static_cast<WGPUPowerPreference>(power_preference),
            .backendType = static_cast<WGPUBackendType>(backend_type),
            .forceFallbackAdapter = force_fallback_adapter,
#if WEBGPU_BACKEND_DAWN
            .compatibilityMode = compatibility_mode,
#endif
        };
    }

    WGPUSurfaceConfiguration SurfaceConfiguration::c_struct() const
    {
        return {
            .nextInChain = next_in_chain->c_struct(),
            .device = device,
            .format = static_cast<WGPUTextureFormat>(format),
            .usage = static_cast<WGPUTextureUsageFlags>(usage),
            .viewFormatCount = view_formats.size(),
            .viewFormats = reinterpret_cast<const WGPUTextureFormat *>(view_formats.data()),
            .alphaMode = static_cast<WGPUCompositeAlphaMode>(alpha_mode),
            .width = width,
            .height = height,
            .presentMode = static_cast<WGPUPresentMode>(present_mode),
        };
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

    std::vector<WGPUFeatureName> Adapter::enumerate_features() const
    {
        const auto feature_count = wgpuAdapterEnumerateFeatures(m_handle, nullptr);

        std::vector<WGPUFeatureName> features(feature_count);
        wgpuAdapterEnumerateFeatures(m_handle, features.data());

        return features;
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

        const auto wgpu_options = options.c_struct();

        wgpuInstanceRequestAdapter(m_handle, &wgpu_options, on_request_ended, handle.get());
        return handle;
    }

    std::expected<Adapter, const char *> Instance::request_adapter(const RequestAdapterOptions &options) const
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
        const auto wgpu_configuration = configuration.c_struct();
        wgpuSurfaceConfigure(m_handle, &wgpu_configuration);
    }

    WGPUSurface Surface::c_ptr() const
    {
        return m_handle;
    }

    Instance create_instance(const InstanceDescriptor &descriptor)
    {
        return Instance{wgpuCreateInstance(descriptor.c_struct())};
    }
}
