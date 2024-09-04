#pragma once

#include <expected>
#include <functional>
#include <memory>
#include <optional>

#include <webgpu/webgpu.h>

namespace wgpu
{
    // RAII Handle Forward Declarations
    class Adapter;
    class Instance;
    class Surface;

    // Enums
    enum class BackendType : uint32_t
    {
        Undefined = WGPUBackendType_Undefined,
        Null      = WGPUBackendType_Null,
        WebGPU    = WGPUBackendType_WebGPU,
        D3D11     = WGPUBackendType_D3D11,
        D3D12     = WGPUBackendType_D3D12,
        Metal     = WGPUBackendType_Metal,
        Vulkan    = WGPUBackendType_Vulkan,
        OpenGL    = WGPUBackendType_OpenGL,
        OpenGLES  = WGPUBackendType_OpenGLES,
        Force32   = WGPUBackendType_Force32,
    };

    enum class CompositeAlphaMode : uint32_t
    {
        Auto            = WGPUCompositeAlphaMode_Auto,
        Opaque          = WGPUCompositeAlphaMode_Opaque,
        Premultiplied   = WGPUCompositeAlphaMode_Premultiplied,
        Unpremultiplied = WGPUCompositeAlphaMode_Unpremultiplied,
        Inherit         = WGPUCompositeAlphaMode_Inherit,
        Force32         = WGPUCompositeAlphaMode_Force32,
    };

    enum class PowerPreference : uint32_t
    {
        Undefined       = WGPUPowerPreference_Undefined,
        LowPower        = WGPUPowerPreference_LowPower,
        HighPerformance = WGPUPowerPreference_HighPerformance,
        Force32         = WGPUPowerPreference_Force32,
    };

    enum class PresentMode : uint32_t
    {
        Fifo        = WGPUPresentMode_Fifo,
        FifoRelaxed = WGPUPresentMode_FifoRelaxed,
        Immediate   = WGPUPresentMode_Immediate,
        Mailbox     = WGPUPresentMode_Mailbox,
        Force32     = WGPUPresentMode_Force32,
    };

    enum class RequestAdapterStatus : uint32_t
    {
        Success     = WGPURequestAdapterStatus_Success,
        Unavailable = WGPURequestAdapterStatus_Unavailable,
        Error       = WGPURequestAdapterStatus_Error,
        Unknown     = WGPURequestAdapterStatus_Unknown,
        Force32     = WGPURequestAdapterStatus_Force32,
    };

    enum class SType : uint32_t
    {
#ifdef WEBGPU_BACKEND_WGPU
        Invalid = WGPUSType_Invalid,
#endif
        SurfaceDescriptorFromMetalLayer                    = WGPUSType_SurfaceDescriptorFromMetalLayer,
        SurfaceDescriptorFromWindowsHWND                   = WGPUSType_SurfaceDescriptorFromWindowsHWND,
        SurfaceDescriptorFromXlibWindow                    = WGPUSType_SurfaceDescriptorFromXlibWindow,
        SurfaceDescriptorFromCanvasHTMLSelector            = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector,
        ShaderModuleSPIRVDescriptor                        = WGPUSType_ShaderModuleSPIRVDescriptor,
        ShaderModuleWGSLDescriptor                         = WGPUSType_ShaderModuleWGSLDescriptor,
        PrimitiveDepthClipControl                          = WGPUSType_PrimitiveDepthClipControl,
        SurfaceDescriptorFromWaylandSurface                = WGPUSType_SurfaceDescriptorFromWaylandSurface,
        SurfaceDescriptorFromAndroidNativeWindow           = WGPUSType_SurfaceDescriptorFromAndroidNativeWindow,
#ifdef WEBGPU_BACKEND_DAWN
        SurfaceDescriptorFromWindowsCoreWindow             = WGPUSType_SurfaceDescriptorFromWindowsCoreWindow,
        ExternalTextureBindingEntry                        = WGPUSType_ExternalTextureBindingEntry,
        ExternalTextureBindingLayout                       = WGPUSType_ExternalTextureBindingLayout,
        SurfaceDescriptorFromWindowsSwapChainPanel         = WGPUSType_SurfaceDescriptorFromWindowsSwapChainPanel,
#endif
#ifdef WEBGPU_BACKEND_WGPU
        SurfaceDescriptorFromXcbWindow                    = WGPUSType_SurfaceDescriptorFromXcbWindow,
#endif
#ifdef WEBGPU_BACKEND_DAWN
        DepthStencilStateDepthWriteDefinedDawn             = WGPUSType_DepthStencilStateDepthWriteDefinedDawn,
        TextureBindingViewDimensionDescriptor              = WGPUSType_TextureBindingViewDimensionDescriptor,
        DawnTextureInternalUsageDescriptor                 = WGPUSType_DawnTextureInternalUsageDescriptor,
        DawnEncoderInternalUsageDescriptor                 = WGPUSType_DawnEncoderInternalUsageDescriptor,
        DawnInstanceDescriptor                             = WGPUSType_DawnInstanceDescriptor,
        DawnCacheDeviceDescriptor                          = WGPUSType_DawnCacheDeviceDescriptor,
        DawnAdapterPropertiesPowerPreference               = WGPUSType_DawnAdapterPropertiesPowerPreference,
        DawnBufferDescriptorErrorInfoFromWireClient        = WGPUSType_DawnBufferDescriptorErrorInfoFromWireClient,
        DawnTogglesDescriptor                              = WGPUSType_DawnTogglesDescriptor,
        DawnShaderModuleSPIRVOptionsDescriptor             = WGPUSType_DawnShaderModuleSPIRVOptionsDescriptor,
        RequestAdapterOptionsLUID                          = WGPUSType_RequestAdapterOptionsLUID,
        RequestAdapterOptionsGetGLProc                     = WGPUSType_RequestAdapterOptionsGetGLProc,
        RequestAdapterOptionsD3D11Device                   = WGPUSType_RequestAdapterOptionsD3D11Device,
        DawnRenderPassColorAttachmentRenderToSingleSampled = WGPUSType_DawnRenderPassColorAttachmentRenderToSingleSampled,
        RenderPassPixelLocalStorage                        = WGPUSType_RenderPassPixelLocalStorage,
        PipelineLayoutPixelLocalStorage                    = WGPUSType_PipelineLayoutPixelLocalStorage,
        BufferHostMappedPointer                            = WGPUSType_BufferHostMappedPointer,
        DawnExperimentalSubgroupLimits                     = WGPUSType_DawnExperimentalSubgroupLimits,
        AdapterPropertiesMemoryHeaps                       = WGPUSType_AdapterPropertiesMemoryHeaps,
        AdapterPropertiesD3D                               = WGPUSType_AdapterPropertiesD3D,
        AdapterPropertiesVk                                = WGPUSType_AdapterPropertiesVk,
        DawnComputePipelineFullSubgroups                   = WGPUSType_DawnComputePipelineFullSubgroups,
        DawnWireWGSLControl                                = WGPUSType_DawnWireWGSLControl,
        DawnWGSLBlocklist                                  = WGPUSType_DawnWGSLBlocklist,
        DrmFormatCapabilities                              = WGPUSType_DrmFormatCapabilities,
        ShaderModuleCompilationOptions                     = WGPUSType_ShaderModuleCompilationOptions,
        ColorTargetStateExpandResolveTextureDawn           = WGPUSType_ColorTargetStateExpandResolveTextureDawn,
        SharedTextureMemoryVkDedicatedAllocationDescriptor = WGPUSType_SharedTextureMemoryVkDedicatedAllocationDescriptor,
        SharedTextureMemoryAHardwareBufferDescriptor       = WGPUSType_SharedTextureMemoryAHardwareBufferDescriptor,
        SharedTextureMemoryDmaBufDescriptor                = WGPUSType_SharedTextureMemoryDmaBufDescriptor,
        SharedTextureMemoryOpaqueFDDescriptor              = WGPUSType_SharedTextureMemoryOpaqueFDDescriptor,
        SharedTextureMemoryZirconHandleDescriptor          = WGPUSType_SharedTextureMemoryZirconHandleDescriptor,
        SharedTextureMemoryDXGISharedHandleDescriptor      = WGPUSType_SharedTextureMemoryDXGISharedHandleDescriptor,
        SharedTextureMemoryD3D11Texture2DDescriptor        = WGPUSType_SharedTextureMemoryD3D11Texture2DDescriptor,
        SharedTextureMemoryIOSurfaceDescriptor             = WGPUSType_SharedTextureMemoryIOSurfaceDescriptor,
        SharedTextureMemoryEGLImageDescriptor              = WGPUSType_SharedTextureMemoryEGLImageDescriptor,
        SharedTextureMemoryInitializedBeginState           = WGPUSType_SharedTextureMemoryInitializedBeginState,
        SharedTextureMemoryInitializedEndState             = WGPUSType_SharedTextureMemoryInitializedEndState,
        SharedTextureMemoryVkImageLayoutBeginState         = WGPUSType_SharedTextureMemoryVkImageLayoutBeginState,
        SharedTextureMemoryVkImageLayoutEndState           = WGPUSType_SharedTextureMemoryVkImageLayoutEndState,
        SharedTextureMemoryD3DSwapchainBeginState          = WGPUSType_SharedTextureMemoryD3DSwapchainBeginState,
        SharedFenceVkSemaphoreOpaqueFDDescriptor           = WGPUSType_SharedFenceVkSemaphoreOpaqueFDDescriptor,
        SharedFenceVkSemaphoreOpaqueFDExportInfo           = WGPUSType_SharedFenceVkSemaphoreOpaqueFDExportInfo,
        SharedFenceVkSemaphoreSyncFDDescriptor             = WGPUSType_SharedFenceVkSemaphoreSyncFDDescriptor,
        SharedFenceVkSemaphoreSyncFDExportInfo             = WGPUSType_SharedFenceVkSemaphoreSyncFDExportInfo,
        SharedFenceVkSemaphoreZirconHandleDescriptor       = WGPUSType_SharedFenceVkSemaphoreZirconHandleDescriptor,
        SharedFenceVkSemaphoreZirconHandleExportInfo       = WGPUSType_SharedFenceVkSemaphoreZirconHandleExportInfo,
        SharedFenceDXGISharedHandleDescriptor              = WGPUSType_SharedFenceDXGISharedHandleDescriptor,
        SharedFenceDXGISharedHandleExportInfo              = WGPUSType_SharedFenceDXGISharedHandleExportInfo,
        SharedFenceMTLSharedEventDescriptor                = WGPUSType_SharedFenceMTLSharedEventDescriptor,
        SharedFenceMTLSharedEventExportInfo                = WGPUSType_SharedFenceMTLSharedEventExportInfo,
        SharedBufferMemoryD3D12ResourceDescriptor          = WGPUSType_SharedBufferMemoryD3D12ResourceDescriptor,
        StaticSamplerBindingLayout                         = WGPUSType_StaticSamplerBindingLayout,
        YCbCrVkDescriptor                                  = WGPUSType_YCbCrVkDescriptor,
        SharedTextureMemoryAHardwareBufferProperties       = WGPUSType_SharedTextureMemoryAHardwareBufferProperties,
        AHardwareBufferProperties                          = WGPUSType_AHardwareBufferProperties,
#endif
        RenderPassDescriptorMaxDrawCount                   = WGPUSType_RenderPassDescriptorMaxDrawCount,
        Force32                                            = WGPUSType_Force32,
    };

    enum class TextureFormat : uint32_t
    {
        Undefined            = WGPUTextureFormat_Undefined,
        R8Unorm              = WGPUTextureFormat_R8Unorm,
        R8Snorm              = WGPUTextureFormat_R8Snorm,
        R8Uint               = WGPUTextureFormat_R8Uint,
        R8Sint               = WGPUTextureFormat_R8Sint,
        R16Uint              = WGPUTextureFormat_R16Uint,
        R16Sint              = WGPUTextureFormat_R16Sint,
        R16Float             = WGPUTextureFormat_R16Float,
        RG8Unorm             = WGPUTextureFormat_RG8Unorm,
        RG8Snorm             = WGPUTextureFormat_RG8Snorm,
        RG8Uint              = WGPUTextureFormat_RG8Uint,
        RG8Sint              = WGPUTextureFormat_RG8Sint,
        R32Float             = WGPUTextureFormat_R32Float,
        R32Uint              = WGPUTextureFormat_R32Uint,
        R32Sint              = WGPUTextureFormat_R32Sint,
        RG16Uint             = WGPUTextureFormat_RG16Uint,
        RG16Sint             = WGPUTextureFormat_RG16Sint,
        RG16Float            = WGPUTextureFormat_RG16Float,
        RGBA8Unorm           = WGPUTextureFormat_RGBA8Unorm,
        RGBA8UnormSrgb       = WGPUTextureFormat_RGBA8UnormSrgb,
        RGBA8Snorm           = WGPUTextureFormat_RGBA8Snorm,
        RGBA8Uint            = WGPUTextureFormat_RGBA8Uint,
        RGBA8Sint            = WGPUTextureFormat_RGBA8Sint,
        BGRA8Unorm           = WGPUTextureFormat_BGRA8Unorm,
        BGRA8UnormSrgb       = WGPUTextureFormat_BGRA8UnormSrgb,
        RGB10A2Uint          = WGPUTextureFormat_RGB10A2Uint,
        RGB10A2Unorm         = WGPUTextureFormat_RGB10A2Unorm,
        RG11B10Ufloat        = WGPUTextureFormat_RG11B10Ufloat,
        RGB9E5Ufloat         = WGPUTextureFormat_RGB9E5Ufloat,
        RG32Float            = WGPUTextureFormat_RG32Float,
        RG32Uint             = WGPUTextureFormat_RG32Uint,
        RG32Sint             = WGPUTextureFormat_RG32Sint,
        RGBA16Uint           = WGPUTextureFormat_RGBA16Uint,
        RGBA16Sint           = WGPUTextureFormat_RGBA16Sint,
        RGBA16Float          = WGPUTextureFormat_RGBA16Float,
        RGBA32Float          = WGPUTextureFormat_RGBA32Float,
        RGBA32Uint           = WGPUTextureFormat_RGBA32Uint,
        RGBA32Sint           = WGPUTextureFormat_RGBA32Sint,
        Stencil8             = WGPUTextureFormat_Stencil8,
        Depth16Unorm         = WGPUTextureFormat_Depth16Unorm,
        Depth24Plus          = WGPUTextureFormat_Depth24Plus,
        Depth24PlusStencil8  = WGPUTextureFormat_Depth24PlusStencil8,
        Depth32Float         = WGPUTextureFormat_Depth32Float,
        Depth32FloatStencil8 = WGPUTextureFormat_Depth32FloatStencil8,
        BC1RGBAUnorm         = WGPUTextureFormat_BC1RGBAUnorm,
        BC1RGBAUnormSrgb     = WGPUTextureFormat_BC1RGBAUnormSrgb,
        BC2RGBAUnorm         = WGPUTextureFormat_BC2RGBAUnorm,
        BC2RGBAUnormSrgb     = WGPUTextureFormat_BC2RGBAUnormSrgb,
        BC3RGBAUnorm         = WGPUTextureFormat_BC3RGBAUnorm,
        BC3RGBAUnormSrgb     = WGPUTextureFormat_BC3RGBAUnormSrgb,
        BC4RUnorm            = WGPUTextureFormat_BC4RUnorm,
        BC4RSnorm            = WGPUTextureFormat_BC4RSnorm,
        BC5RGUnorm           = WGPUTextureFormat_BC5RGUnorm,
        BC5RGSnorm           = WGPUTextureFormat_BC5RGSnorm,
        BC6HRGBUfloat        = WGPUTextureFormat_BC6HRGBUfloat,
        BC6HRGBFloat         = WGPUTextureFormat_BC6HRGBFloat,
        BC7RGBAUnorm         = WGPUTextureFormat_BC7RGBAUnorm,
        BC7RGBAUnormSrgb     = WGPUTextureFormat_BC7RGBAUnormSrgb,
        ETC2RGB8Unorm        = WGPUTextureFormat_ETC2RGB8Unorm,
        ETC2RGB8UnormSrgb    = WGPUTextureFormat_ETC2RGB8UnormSrgb,
        ETC2RGB8A1Unorm      = WGPUTextureFormat_ETC2RGB8A1Unorm,
        ETC2RGB8A1UnormSrgb  = WGPUTextureFormat_ETC2RGB8A1UnormSrgb,
        ETC2RGBA8Unorm       = WGPUTextureFormat_ETC2RGBA8Unorm,
        ETC2RGBA8UnormSrgb   = WGPUTextureFormat_ETC2RGBA8UnormSrgb,
        EACR11Unorm          = WGPUTextureFormat_EACR11Unorm,
        EACR11Snorm          = WGPUTextureFormat_EACR11Snorm,
        EACRG11Unorm         = WGPUTextureFormat_EACRG11Unorm,
        EACRG11Snorm         = WGPUTextureFormat_EACRG11Snorm,
        ASTC4x4Unorm         = WGPUTextureFormat_ASTC4x4Unorm,
        ASTC4x4UnormSrgb     = WGPUTextureFormat_ASTC4x4UnormSrgb,
        ASTC5x4Unorm         = WGPUTextureFormat_ASTC5x4Unorm,
        ASTC5x4UnormSrgb     = WGPUTextureFormat_ASTC5x4UnormSrgb,
        ASTC5x5Unorm         = WGPUTextureFormat_ASTC5x5Unorm,
        ASTC5x5UnormSrgb     = WGPUTextureFormat_ASTC5x5UnormSrgb,
        ASTC6x5Unorm         = WGPUTextureFormat_ASTC6x5Unorm,
        ASTC6x5UnormSrgb     = WGPUTextureFormat_ASTC6x5UnormSrgb,
        ASTC6x6Unorm         = WGPUTextureFormat_ASTC6x6Unorm,
        ASTC6x6UnormSrgb     = WGPUTextureFormat_ASTC6x6UnormSrgb,
        ASTC8x5Unorm         = WGPUTextureFormat_ASTC8x5Unorm,
        ASTC8x5UnormSrgb     = WGPUTextureFormat_ASTC8x5UnormSrgb,
        ASTC8x6Unorm         = WGPUTextureFormat_ASTC8x6Unorm,
        ASTC8x6UnormSrgb     = WGPUTextureFormat_ASTC8x6UnormSrgb,
        ASTC8x8Unorm         = WGPUTextureFormat_ASTC8x8Unorm,
        ASTC8x8UnormSrgb     = WGPUTextureFormat_ASTC8x8UnormSrgb,
        ASTC10x5Unorm        = WGPUTextureFormat_ASTC10x5Unorm,
        ASTC10x5UnormSrgb    = WGPUTextureFormat_ASTC10x5UnormSrgb,
        ASTC10x6Unorm        = WGPUTextureFormat_ASTC10x6Unorm,
        ASTC10x6UnormSrgb    = WGPUTextureFormat_ASTC10x6UnormSrgb,
        ASTC10x8Unorm        = WGPUTextureFormat_ASTC10x8Unorm,
        ASTC10x8UnormSrgb    = WGPUTextureFormat_ASTC10x8UnormSrgb,
        ASTC10x10Unorm       = WGPUTextureFormat_ASTC10x10Unorm,
        ASTC10x10UnormSrgb   = WGPUTextureFormat_ASTC10x10UnormSrgb,
        ASTC12x10Unorm       = WGPUTextureFormat_ASTC12x10Unorm,
        ASTC12x10UnormSrgb   = WGPUTextureFormat_ASTC12x10UnormSrgb,
        ASTC12x12Unorm       = WGPUTextureFormat_ASTC12x12Unorm,
        ASTC12x12UnormSrgb   = WGPUTextureFormat_ASTC12x12UnormSrgb,
        Force32              = WGPUTextureFormat_Force32,
    };

    enum class TextureUsageFlags : uint32_t
    {
        None             = WGPUTextureUsage_None,
        CopySrc          = WGPUTextureUsage_CopySrc,
        CopyDst          = WGPUTextureUsage_CopyDst,
        TextureBinding   = WGPUTextureUsage_TextureBinding,
        StorageBinding   = WGPUTextureUsage_StorageBinding,
        RenderAttachment = WGPUTextureUsage_RenderAttachment,
        Force32          = WGPUTextureUsage_Force32,
    };

    TextureUsageFlags operator|(TextureUsageFlags lhs, TextureUsageFlags rhs);
    TextureUsageFlags & operator|=(TextureUsageFlags& lhs, TextureUsageFlags rhs);
    TextureUsageFlags operator&(TextureUsageFlags lhs, TextureUsageFlags rhs);
    TextureUsageFlags & operator&=(TextureUsageFlags& lhs, TextureUsageFlags rhs);
    bool operator==(TextureUsageFlags lhs, TextureUsageFlags rhs);

    // Structs
    struct ChainedStruct
    {
        const ChainedStruct *next_in_chain;
        SType s_type;
    };

    struct InstanceDescriptor
    {
        const ChainedStruct *next_in_chain;
#ifdef WEBGPU_BACKEND_DAWN
        WGPUInstanceFeatures features;
#endif
    };

    struct RequestAdapterOptions
    {
        const ChainedStruct *next_in_chain;
        // TODO: std::optional<Surface> compatible_surface;
        WGPUSurface compatible_surface;
        PowerPreference power_preference;
        BackendType backend_type;
        bool force_fallback_adapter;
    };

    struct SurfaceConfiguration
    {
        const ChainedStruct *next_in_chain;
        // TODO: Device device
        WGPUDevice device;
        TextureFormat format;
        TextureUsageFlags usage;
        // TODO: Use vector
        size_t view_format_count;
        const TextureFormat *view_formats;
        CompositeAlphaMode alpha_mode;
        uint32_t width;
        uint32_t height;
        PresentMode present_mode;
    };

    // Callback Types
    using RequestAdapterCallback = std::function<void(RequestAdapterStatus status, const Adapter &adapter,
        const char *message)>;

    // RAII Handles
    class Adapter
    {
    public:
        explicit Adapter(const WGPUAdapter &handle);
        ~Adapter();

        Adapter(const Adapter &other);
        Adapter(Adapter &&other) noexcept;
        Adapter & operator=(const Adapter &other);
        Adapter & operator=(Adapter &&other) noexcept;

        [[nodiscard]] std::vector<WGPUFeatureName> enumerate_features() const;
        // TODO: get_limits
        // TODO: get_properties
        // TODO: has_feature
        // TODO: request_device

        [[nodiscard]] WGPUAdapter get() const;

    private:
        WGPUAdapter m_handle{nullptr};
    };

    class Instance
    {
    public:
        explicit Instance(const WGPUInstance &handle);
        ~Instance();

        Instance(const Instance &other);
        Instance(Instance &&other) noexcept;
        Instance & operator=(const Instance &other);
        Instance & operator=(Instance &&other) noexcept;

        // TODO: create_surface

        void process_events() const;

        std::unique_ptr<RequestAdapterCallback> request_adapter(RequestAdapterOptions options,
            RequestAdapterCallback &&callback) const;
        [[nodiscard]] std::expected<Adapter, const char *> request_adapter(const RequestAdapterOptions &options) const;

        [[nodiscard]] WGPUInstance get() const;

    private:
        WGPUInstance m_handle{nullptr};
    };

    class Surface
    {
    public:
        explicit Surface(const WGPUSurface &handle);
        ~Surface();

        Surface(const Surface &other);
        Surface(Surface &&other) noexcept;
        Surface & operator=(const Surface &other);
        Surface & operator=(Surface &&other) noexcept;

        void configure(SurfaceConfiguration configuration) const;

        [[nodiscard]] WGPUSurface get() const;

    private:
        WGPUSurface m_handle{nullptr};
    };

    // Non-member Functions
    Instance create_instance(InstanceDescriptor descriptor);
}
