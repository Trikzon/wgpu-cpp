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
            wgpuAdapterReference(m_handle);
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
                wgpuAdapterReference(m_handle);
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

    WGPUAdapter Adapter::get() const
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
            wgpuInstanceReference(m_handle);
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
                wgpuInstanceReference(m_handle);
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

    std::unique_ptr<RequestAdapterCallback> Instance::request_adapter(RequestAdapterOptions options,
        RequestAdapterCallback&& callback) const
    {
        auto handle = std::make_unique<RequestAdapterCallback>(callback);
        static auto on_request_ended = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, const char *message,
            void *user_data) -> void
        {
            const RequestAdapterCallback& callback = *static_cast<RequestAdapterCallback*>(user_data);
            callback(static_cast<RequestAdapterStatus>(status), Adapter{adapter}, message);
        };

        wgpuInstanceRequestAdapter(m_handle, reinterpret_cast<WGPURequestAdapterOptions *>(&options),
            on_request_ended, handle.get());
        return handle;
    }

    std::expected<Adapter, const char *> Instance::request_adapter(const RequestAdapterOptions &options) const
    {
        std::expected<Adapter, const char *> result = std::unexpected("Request did not end.");
        bool request_ended = false;

        auto on_adapter_request_ended = [&result, &request_ended](RequestAdapterStatus status, const Adapter& adapter, const char *message)
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

    WGPUInstance Instance::get() const
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
            wgpuSurfaceReference(m_handle);
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
                wgpuSurfaceReference(m_handle);
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

    void Surface::configure(SurfaceConfiguration configuration) const
    {
        wgpuSurfaceConfigure(m_handle, reinterpret_cast<WGPUSurfaceConfiguration *>(&configuration));
    }

    WGPUSurface Surface::get() const
    {
        return m_handle;
    }

    Instance create_instance(InstanceDescriptor descriptor)
    {
        return Instance{wgpuCreateInstance(reinterpret_cast<WGPUInstanceDescriptor *>(&descriptor))};
    }
}
