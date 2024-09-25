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
    class BindGroup;
    class BindGroupLayout;
    class Buffer;
    class CommandBuffer;
    class CommandEncoder;
    class Device;
    class PipelineLayout;
    class Queue;
    class RenderPassEncoder;
    class RenderPipeline;
    class Sampler;
    class ShaderModule;
    class Surface;
    class Texture;
    class TextureView;

    // Struct Forward Declarations
    struct AdapterProperties;
    struct BindGroupDescriptor;
    struct BindGroupEntry;
    struct BindGroupLayoutDescriptor;
    struct BindGroupLayoutEntry;
    struct BlendComponent;
    struct BlendState;
    struct BufferBindingLayout;
    struct BufferDescriptor;
    struct ChainedStruct;
    struct ChainedStructOut;
    struct Color;
    struct ColorTargetState;
    struct CommandBufferDescriptor;
    struct CommandEncoderDescriptor;
    struct ConstantEntry;
    struct DepthStencilState;
    struct DeviceDescriptor;
    struct Extent3D;
    struct FragmentState;
    struct ImageCopyTexture;
#ifdef WEBGPU_BACKEND_DAWN
    struct InstanceFeatures;
#endif
    struct InstanceDescriptor;
    struct Limits;
    struct MultisampleState;
    struct Origin3D;
    struct PipelineLayoutDescriptor;
    struct PrimitiveState;
    struct QueueDescriptor;
    struct RequestAdapterOptions;
    struct RequiredLimits;
    struct RenderPassColorAttachment;
    struct RenderPassDepthStencilAttachment;
    struct RenderPassDescriptor;
    struct RenderPipelineDescriptor;
    struct SamplerBindingLayout;
    struct SamplerDescriptor;
    struct ShaderModuleDescriptor;
    struct ShaderModuleSPIRVDescriptor;
    struct ShaderModuleWGSLDescriptor;
    struct StencilFaceState;
    struct StorageTextureBindingLayout;
    struct SupportedLimits;
    struct SurfaceCapabilities;
    struct SurfaceConfiguration;
    struct SurfaceTexture;
    struct TextureBindingLayout;
    struct TextureDataLayout;
    struct TextureDescriptor;
    struct TextureViewDescriptor;
    struct VertexAttribute;
    struct VertexBufferLayout;
    struct VertexState;

    // Enums
    enum class AdapterType : uint32_t
    {
        DiscreteGPU   = WGPUAdapterType_DiscreteGPU,
        IntegratedGPU = WGPUAdapterType_IntegratedGPU,
        CPU           = WGPUAdapterType_CPU,
        Unknown       = WGPUAdapterType_Unknown,
    };

    enum class AddressMode : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined    = WGPUAddressMode_Undefined,
#endif
        ClampToEdge  = WGPUAddressMode_ClampToEdge,
        Repeat       = WGPUAddressMode_Repeat,
        MirrorRepeat = WGPUAddressMode_MirrorRepeat,
    };

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
    };

    enum class BlendFactor : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined         = WGPUBlendFactor_Undefined,
#endif
        Zero              = WGPUBlendFactor_Zero,
        One               = WGPUBlendFactor_One,
        Src               = WGPUBlendFactor_Src,
        OneMinusSrc       = WGPUBlendFactor_OneMinusSrc,
        SrcAlpha          = WGPUBlendFactor_SrcAlpha,
        OneMinusSrcAlpha  = WGPUBlendFactor_OneMinusSrcAlpha,
        Dst               = WGPUBlendFactor_Dst,
        OneMinusDst       = WGPUBlendFactor_OneMinusDst,
        DstAlpha          = WGPUBlendFactor_DstAlpha,
        OneMinusDstAlpha  = WGPUBlendFactor_OneMinusDstAlpha,
        SrcAlphaSaturated = WGPUBlendFactor_SrcAlphaSaturated,
        Constant          = WGPUBlendFactor_Constant,
        OneMinusConstant  = WGPUBlendFactor_OneMinusConstant,
#ifdef WEBGPU_BACKEND_DAWN
        Src1              = WGPUBlendFactor_Src1,
        OneMinusSrc1      = WGPUBlendFactor_OneMinusSrc1,
        Src1Alpha         = WGPUBlendFactor_Src1Alpha,
        OneMinusSrc1Alpha = WGPUBlendFactor_OneMinusSrc1Alpha,
#endif
    };

    enum class BlendOperation : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined       = WGPUBlendOperation_Undefined,
#endif
        Add             = WGPUBlendOperation_Add,
        Subtract        = WGPUBlendOperation_Subtract,
        ReverseSubtract = WGPUBlendOperation_ReverseSubtract,
        Min             = WGPUBlendOperation_Min,
        Max             = WGPUBlendOperation_Max,
    };

    enum class BufferBindingType : uint32_t
    {
        Undefined       = WGPUBufferBindingType_Undefined,
        Uniform         = WGPUBufferBindingType_Uniform,
        Storage         = WGPUBufferBindingType_Storage,
        ReadOnlyStorage = WGPUBufferBindingType_ReadOnlyStorage,
    };

    enum class BufferMapAsyncStatus : uint32_t
    {
        Success                 = WGPUBufferMapAsyncStatus_Success,
#ifdef WEBGPU_BACKEND_DAWN
        InstanceDropped         = WGPUBufferMapAsyncStatus_InstanceDropped,
#endif
        ValidationError         = WGPUBufferMapAsyncStatus_ValidationError,
        Unknown                 = WGPUBufferMapAsyncStatus_Unknown,
        DeviceLost              = WGPUBufferMapAsyncStatus_DeviceLost,
        DestroyedBeforeCallback = WGPUBufferMapAsyncStatus_DestroyedBeforeCallback,
        UnmappedBeforeCallback  = WGPUBufferMapAsyncStatus_UnmappedBeforeCallback,
        MappingAlreadyPending   = WGPUBufferMapAsyncStatus_MappingAlreadyPending,
        OffsetOutOfRange        = WGPUBufferMapAsyncStatus_OffsetOutOfRange,
        SizeOutOfRange          = WGPUBufferMapAsyncStatus_SizeOutOfRange,
    };

    enum class BufferUsageFlags : uint32_t
    {
        None         = WGPUBufferUsage_None,
        MapRead      = WGPUBufferUsage_MapRead,
        MapWrite     = WGPUBufferUsage_MapWrite,
        CopySrc      = WGPUBufferUsage_CopySrc,
        CopyDst      = WGPUBufferUsage_CopyDst,
        Index        = WGPUBufferUsage_Index,
        Vertex       = WGPUBufferUsage_Vertex,
        Uniform      = WGPUBufferUsage_Uniform,
        Storage      = WGPUBufferUsage_Storage,
        Indirect     = WGPUBufferUsage_Indirect,
        QueryResolve = WGPUBufferUsage_QueryResolve,
    };

    BufferUsageFlags operator|(BufferUsageFlags lhs, BufferUsageFlags rhs);
    BufferUsageFlags & operator|=(BufferUsageFlags& lhs, BufferUsageFlags rhs);
    BufferUsageFlags operator&(BufferUsageFlags lhs, BufferUsageFlags rhs);
    BufferUsageFlags & operator&=(BufferUsageFlags& lhs, BufferUsageFlags rhs);
    bool operator==(BufferUsageFlags lhs, BufferUsageFlags rhs);

    enum class ColorWriteMaskFlags : uint32_t
    {
        None = WGPUColorWriteMask_None,
        Red = WGPUColorWriteMask_Red,
        Green = WGPUColorWriteMask_Green,
        Blue = WGPUColorWriteMask_Blue,
        Alpha = WGPUColorWriteMask_Alpha,
        All = WGPUColorWriteMask_All,
    };

    ColorWriteMaskFlags operator|(ColorWriteMaskFlags lhs, ColorWriteMaskFlags rhs);
    ColorWriteMaskFlags & operator|=(ColorWriteMaskFlags& lhs, ColorWriteMaskFlags rhs);
    ColorWriteMaskFlags operator&(ColorWriteMaskFlags lhs, ColorWriteMaskFlags rhs);
    ColorWriteMaskFlags & operator&=(ColorWriteMaskFlags& lhs, ColorWriteMaskFlags rhs);
    bool operator==(ColorWriteMaskFlags lhs, ColorWriteMaskFlags rhs);

    enum class CompareFunction : uint32_t
    {
        Undefined    = WGPUCompareFunction_Undefined,
        Never        = WGPUCompareFunction_Never,
        Less         = WGPUCompareFunction_Less,
        Equal        = WGPUCompareFunction_Equal,
        LessEqual    = WGPUCompareFunction_LessEqual,
        Greater      = WGPUCompareFunction_Greater,
        NotEqual     = WGPUCompareFunction_NotEqual,
        GreaterEqual = WGPUCompareFunction_GreaterEqual,
        Always       = WGPUCompareFunction_Always,
    };

    enum class CompositeAlphaMode : uint32_t
    {
        Auto            = WGPUCompositeAlphaMode_Auto,
        Opaque          = WGPUCompositeAlphaMode_Opaque,
        Premultiplied   = WGPUCompositeAlphaMode_Premultiplied,
        Unpremultiplied = WGPUCompositeAlphaMode_Unpremultiplied,
        Inherit         = WGPUCompositeAlphaMode_Inherit,
    };

    enum class CullMode : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined = WGPUCullMode_Undefined,
#endif
        None      = WGPUCullMode_None,
        Front     = WGPUCullMode_Front,
        Back      = WGPUCullMode_Back,
    };

    enum class FeatureName : uint32_t
    {
        Undefined                                      = WGPUFeatureName_Undefined,
        DepthClipControl                               = WGPUFeatureName_DepthClipControl,
        Depth32FloatStencil8                           = WGPUFeatureName_Depth32FloatStencil8,
        TimestampQuery                                 = WGPUFeatureName_TimestampQuery,
        TextureCompressionBC                           = WGPUFeatureName_TextureCompressionBC,
        TextureCompressionETC2                         = WGPUFeatureName_TextureCompressionETC2,
        TextureCompressionASTC                         = WGPUFeatureName_TextureCompressionASTC,
        IndirectFirstInstance                          = WGPUFeatureName_IndirectFirstInstance,
        ShaderF16                                      = WGPUFeatureName_ShaderF16,
        RG11B10UfloatRenderable                        = WGPUFeatureName_RG11B10UfloatRenderable,
        BGRA8UnormStorage                              = WGPUFeatureName_BGRA8UnormStorage,
        Float32Filterable                              = WGPUFeatureName_Float32Filterable,
#ifdef WEBGPU_BACKEND_DAWN
        DawnInternalUsages                             = WGPUFeatureName_DawnInternalUsages,
        DawnMultiPlanarFormats                         = WGPUFeatureName_DawnMultiPlanarFormats,
        DawnNative                                     = WGPUFeatureName_DawnNative,
        ChromiumExperimentalTimestampQueryInsidePasses = WGPUFeatureName_ChromiumExperimentalTimestampQueryInsidePasses,
        ImplicitDeviceSynchronization                  = WGPUFeatureName_ImplicitDeviceSynchronization,
        SurfaceCapabilities                            = WGPUFeatureName_SurfaceCapabilities,
        TransientAttachments                           = WGPUFeatureName_TransientAttachments,
        MSAARenderToSingleSampled                      = WGPUFeatureName_MSAARenderToSingleSampled,
        DualSourceBlending                             = WGPUFeatureName_DualSourceBlending,
        D3D11MultithreadProtected                      = WGPUFeatureName_D3D11MultithreadProtected,
        ANGLETextureSharing                            = WGPUFeatureName_ANGLETextureSharing,
        ChromiumExperimentalSubgroups                  = WGPUFeatureName_ChromiumExperimentalSubgroups,
        ChromiumExperimentalSubgroupUniformControlFlow = WGPUFeatureName_ChromiumExperimentalSubgroupUniformControlFlow,
        PixelLocalStorageCoherent                      = WGPUFeatureName_PixelLocalStorageCoherent,
        PixelLocalStorageNonCoherent                   = WGPUFeatureName_PixelLocalStorageNonCoherent,
        Unorm16TextureFormats                          = WGPUFeatureName_Unorm16TextureFormats,
        Snorm16TextureFormats                          = WGPUFeatureName_Snorm16TextureFormats,
        MultiPlanarFormatExtendedUsages                = WGPUFeatureName_MultiPlanarFormatExtendedUsages,
        MultiPlanarFormatP010                          = WGPUFeatureName_MultiPlanarFormatP010,
        HostMappedPointer                              = WGPUFeatureName_HostMappedPointer,
        MultiPlanarRenderTargets                       = WGPUFeatureName_MultiPlanarRenderTargets,
        MultiPlanarFormatNv12a                         = WGPUFeatureName_MultiPlanarFormatNv12a,
        FramebufferFetch                               = WGPUFeatureName_FramebufferFetch,
        BufferMapExtendedUsages                        = WGPUFeatureName_BufferMapExtendedUsages,
        AdapterPropertiesMemoryHeaps                   = WGPUFeatureName_AdapterPropertiesMemoryHeaps,
        AdapterPropertiesD3D                           = WGPUFeatureName_AdapterPropertiesD3D,
        AdapterPropertiesVk                            = WGPUFeatureName_AdapterPropertiesVk,
        R8UnormStorage                                 = WGPUFeatureName_R8UnormStorage,
        FormatCapabilities                             = WGPUFeatureName_FormatCapabilities,
        DrmFormatCapabilities                          = WGPUFeatureName_DrmFormatCapabilities,
        Norm16TextureFormats                           = WGPUFeatureName_Norm16TextureFormats,
        MultiPlanarFormatNv16                          = WGPUFeatureName_MultiPlanarFormatNv16,
        MultiPlanarFormatNv24                          = WGPUFeatureName_MultiPlanarFormatNv24,
        MultiPlanarFormatP210                          = WGPUFeatureName_MultiPlanarFormatP210,
        MultiPlanarFormatP410                          = WGPUFeatureName_MultiPlanarFormatP410,
        SharedTextureMemoryVkDedicatedAllocation       = WGPUFeatureName_SharedTextureMemoryVkDedicatedAllocation,
        SharedTextureMemoryAHardwareBuffer             = WGPUFeatureName_SharedTextureMemoryAHardwareBuffer,
        SharedTextureMemoryDmaBuf                      = WGPUFeatureName_SharedTextureMemoryDmaBuf,
        SharedTextureMemoryOpaqueFD                    = WGPUFeatureName_SharedTextureMemoryOpaqueFD,
        SharedTextureMemoryZirconHandle                = WGPUFeatureName_SharedTextureMemoryZirconHandle,
        SharedTextureMemoryDXGISharedHandle            = WGPUFeatureName_SharedTextureMemoryDXGISharedHandle,
        SharedTextureMemoryD3D11Texture2D              = WGPUFeatureName_SharedTextureMemoryD3D11Texture2D,
        SharedTextureMemoryIOSurface                   = WGPUFeatureName_SharedTextureMemoryIOSurface,
        SharedTextureMemoryEGLImage                    = WGPUFeatureName_SharedTextureMemoryEGLImage,
        SharedFenceVkSemaphoreOpaqueFD                 = WGPUFeatureName_SharedFenceVkSemaphoreOpaqueFD,
        SharedFenceVkSemaphoreSyncFD                   = WGPUFeatureName_SharedFenceVkSemaphoreSyncFD,
        SharedFenceVkSemaphoreZirconHandle             = WGPUFeatureName_SharedFenceVkSemaphoreZirconHandle,
        SharedFenceDXGISharedHandle                    = WGPUFeatureName_SharedFenceDXGISharedHandle,
        SharedFenceMTLSharedEvent                      = WGPUFeatureName_SharedFenceMTLSharedEvent,
        SharedBufferMemoryD3D12Resource                = WGPUFeatureName_SharedBufferMemoryD3D12Resource,
        StaticSamplers                                 = WGPUFeatureName_StaticSamplers,
        YCbCrVulkanSamplers                            = WGPUFeatureName_YCbCrVulkanSamplers,
        ShaderModuleCompilationOptions                 = WGPUFeatureName_ShaderModuleCompilationOptions,
        DawnLoadResolveTexture                         = WGPUFeatureName_DawnLoadResolveTexture,
#endif
    };

    enum class FilterMode : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined = WGPUFilterMode_Undefined,
#endif
        Nearest = WGPUFilterMode_Nearest,
        Linear = WGPUFilterMode_Linear,
    };

    enum class FrontFace : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined = WGPUFrontFace_Undefined,
#endif
        CCW       = WGPUFrontFace_CCW,
        CW        = WGPUFrontFace_CW,
    };

    enum class IndexFormat : uint32_t
    {
        Undefined = WGPUIndexFormat_Undefined,
        Uint16    = WGPUIndexFormat_Uint16,
        Uint32    = WGPUIndexFormat_Uint32,
    };

    enum class LoadOp : uint32_t
    {
        Undefined            = WGPULoadOp_Undefined,
        Clear                = WGPULoadOp_Clear,
        Load                 = WGPULoadOp_Load,
#ifdef WEBGPU_BACKEND_DAWN
        ExpandResolveTexture = WGPULoadOp_ExpandResolveTexture,
#endif
    };

    enum class MapModeFlags : uint32_t
    {
        None  = WGPUMapMode_None,
        Read  = WGPUMapMode_Read,
        Write = WGPUMapMode_Write,
    };

    MapModeFlags operator|(MapModeFlags lhs, MapModeFlags rhs);
    MapModeFlags & operator|=(MapModeFlags& lhs, MapModeFlags rhs);
    MapModeFlags operator&(MapModeFlags lhs, MapModeFlags rhs);
    MapModeFlags & operator&=(MapModeFlags& lhs, MapModeFlags rhs);
    bool operator==(MapModeFlags lhs, MapModeFlags rhs);

    enum class MipmapFilterMode : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined = WGPUMipmapFilterMode_Undefined,
#endif
        Nearest = WGPUMipmapFilterMode_Nearest,
        Linear = WGPUMipmapFilterMode_Linear,
    };

    enum class PowerPreference : uint32_t
    {
        Undefined       = WGPUPowerPreference_Undefined,
        LowPower        = WGPUPowerPreference_LowPower,
        HighPerformance = WGPUPowerPreference_HighPerformance,
    };

    enum class PresentMode : uint32_t
    {
        Fifo        = WGPUPresentMode_Fifo,
        FifoRelaxed = WGPUPresentMode_FifoRelaxed,
        Immediate   = WGPUPresentMode_Immediate,
        Mailbox     = WGPUPresentMode_Mailbox,
    };

    enum class PrimitiveTopology : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined     = WGPUPrimitiveTopology_Undefined,
#endif
        PointList     = WGPUPrimitiveTopology_PointList,
        LineList      = WGPUPrimitiveTopology_LineList,
        LineStrip     = WGPUPrimitiveTopology_LineStrip,
        TriangleList  = WGPUPrimitiveTopology_TriangleList,
        TriangleStrip = WGPUPrimitiveTopology_TriangleStrip,
    };

    enum class RequestAdapterStatus : uint32_t
    {
        Success         = WGPURequestAdapterStatus_Success,
#ifdef WEBGPU_BACKEND_DAWN
        InstanceDropped = WGPURequestAdapterStatus_InstanceDropped,
#endif
        Unavailable     = WGPURequestAdapterStatus_Unavailable,
        Error           = WGPURequestAdapterStatus_Error,
        Unknown         = WGPURequestAdapterStatus_Unknown,
    };

    enum class RequestDeviceStatus : uint32_t
    {
        Success         = WGPURequestDeviceStatus_Success,
#ifdef WEBGPU_BACKEND_DAWN
        InstanceDropped = WGPURequestDeviceStatus_InstanceDropped,
#endif
        Error           = WGPURequestDeviceStatus_Error,
        Unknown         = WGPURequestDeviceStatus_Unknown,
    };

    enum class SamplerBindingType : uint32_t
    {
        Undefined    = WGPUSamplerBindingType_Undefined,
        Filtering    = WGPUSamplerBindingType_Filtering,
        NonFiltering = WGPUSamplerBindingType_NonFiltering,
        Comparison   = WGPUSamplerBindingType_Comparison,
    };

    enum class ShaderStageFlags : uint32_t
    {
        None     = WGPUShaderStage_None,
        Vertex   = WGPUShaderStage_Vertex,
        Fragment = WGPUShaderStage_Fragment,
        Compute  = WGPUShaderStage_Compute,
    };

    ShaderStageFlags operator|(ShaderStageFlags lhs, ShaderStageFlags rhs);
    ShaderStageFlags & operator|=(ShaderStageFlags& lhs, ShaderStageFlags rhs);
    ShaderStageFlags operator&(ShaderStageFlags lhs, ShaderStageFlags rhs);
    ShaderStageFlags & operator&=(ShaderStageFlags& lhs, ShaderStageFlags rhs);
    bool operator==(ShaderStageFlags lhs, ShaderStageFlags rhs);

    enum class StencilOperation : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined      = WGPUStencilOperation_Undefined,
#endif
        Keep           = WGPUStencilOperation_Keep,
        Zero           = WGPUStencilOperation_Zero,
        Replace        = WGPUStencilOperation_Replace,
        Invert         = WGPUStencilOperation_Invert,
        IncrementClamp = WGPUStencilOperation_IncrementClamp,
        DecrementClamp = WGPUStencilOperation_DecrementClamp,
        IncrementWrap  = WGPUStencilOperation_IncrementWrap,
        DecrementWrap  = WGPUStencilOperation_DecrementWrap,
    };

    enum class StorageTextureAccess : uint32_t
    {
        Undefined = WGPUStorageTextureAccess_Undefined,
        WriteOnly = WGPUStorageTextureAccess_WriteOnly,
        ReadOnly  = WGPUStorageTextureAccess_ReadOnly,
        ReadWrite = WGPUStorageTextureAccess_ReadWrite,
    };

    enum class StoreOp : uint32_t
    {
        Undefined = WGPUStoreOp_Undefined,
        Store     = WGPUStoreOp_Store,
        Discard   = WGPUStoreOp_Discard,
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
#ifdef WEBGPU_BACKEND_WGPU
        SurfaceDescriptorFromXcbWindow                     = WGPUSType_SurfaceDescriptorFromXcbWindow,
#endif
#ifdef WEBGPU_BACKEND_DAWN
        SurfaceDescriptorFromWindowsCoreWindow             = WGPUSType_SurfaceDescriptorFromWindowsCoreWindow,
        ExternalTextureBindingEntry                        = WGPUSType_ExternalTextureBindingEntry,
        ExternalTextureBindingLayout                       = WGPUSType_ExternalTextureBindingLayout,
        SurfaceDescriptorFromWindowsSwapChainPanel         = WGPUSType_SurfaceDescriptorFromWindowsSwapChainPanel,
#endif
        RenderPassDescriptorMaxDrawCount                   = WGPUSType_RenderPassDescriptorMaxDrawCount,
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
    };

    enum class SurfaceGetCurrentTextureStatus : uint32_t
    {
        Success     = WGPUSurfaceGetCurrentTextureStatus_Success,
        Timeout     = WGPUSurfaceGetCurrentTextureStatus_Timeout,
        Outdated    = WGPUSurfaceGetCurrentTextureStatus_Outdated,
        Lost        = WGPUSurfaceGetCurrentTextureStatus_Lost,
        OutOfMemory = WGPUSurfaceGetCurrentTextureStatus_OutOfMemory,
        DeviceLost  = WGPUSurfaceGetCurrentTextureStatus_DeviceLost,
#ifdef WEBGPU_BACKEND_DAWN
        Error       = WGPUSurfaceGetCurrentTextureStatus_Error,
#endif
    };

    enum class TextureAspect : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined   = WGPUTextureAspect_Undefined,
#endif
        All         = WGPUTextureAspect_All,
        StencilOnly = WGPUTextureAspect_StencilOnly,
        DepthOnly   = WGPUTextureAspect_DepthOnly,
#ifdef WEBGPU_BACKEND_DAWN
        Plane0Only  = WGPUTextureAspect_Plane0Only,
        Plane1Only  = WGPUTextureAspect_Plane1Only,
        Plane2Only  = WGPUTextureAspect_Plane2Only,
#endif
    };

    enum class TextureDimension : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined = WGPUTextureDimension_Undefined,
#endif
        _1D       = WGPUTextureDimension_1D,
        _2D       = WGPUTextureDimension_2D,
        _3D       = WGPUTextureDimension_3D,
    };

    enum class TextureFormat : uint32_t
    {
        Undefined                   = WGPUTextureFormat_Undefined,
        R8Unorm                     = WGPUTextureFormat_R8Unorm,
        R8Snorm                     = WGPUTextureFormat_R8Snorm,
        R8Uint                      = WGPUTextureFormat_R8Uint,
        R8Sint                      = WGPUTextureFormat_R8Sint,
        R16Uint                     = WGPUTextureFormat_R16Uint,
        R16Sint                     = WGPUTextureFormat_R16Sint,
        R16Float                    = WGPUTextureFormat_R16Float,
        RG8Unorm                    = WGPUTextureFormat_RG8Unorm,
        RG8Snorm                    = WGPUTextureFormat_RG8Snorm,
        RG8Uint                     = WGPUTextureFormat_RG8Uint,
        RG8Sint                     = WGPUTextureFormat_RG8Sint,
        R32Float                    = WGPUTextureFormat_R32Float,
        R32Uint                     = WGPUTextureFormat_R32Uint,
        R32Sint                     = WGPUTextureFormat_R32Sint,
        RG16Uint                    = WGPUTextureFormat_RG16Uint,
        RG16Sint                    = WGPUTextureFormat_RG16Sint,
        RG16Float                   = WGPUTextureFormat_RG16Float,
        RGBA8Unorm                  = WGPUTextureFormat_RGBA8Unorm,
        RGBA8UnormSrgb              = WGPUTextureFormat_RGBA8UnormSrgb,
        RGBA8Snorm                  = WGPUTextureFormat_RGBA8Snorm,
        RGBA8Uint                   = WGPUTextureFormat_RGBA8Uint,
        RGBA8Sint                   = WGPUTextureFormat_RGBA8Sint,
        BGRA8Unorm                  = WGPUTextureFormat_BGRA8Unorm,
        BGRA8UnormSrgb              = WGPUTextureFormat_BGRA8UnormSrgb,
        RGB10A2Uint                 = WGPUTextureFormat_RGB10A2Uint,
        RGB10A2Unorm                = WGPUTextureFormat_RGB10A2Unorm,
        RG11B10Ufloat               = WGPUTextureFormat_RG11B10Ufloat,
        RGB9E5Ufloat                = WGPUTextureFormat_RGB9E5Ufloat,
        RG32Float                   = WGPUTextureFormat_RG32Float,
        RG32Uint                    = WGPUTextureFormat_RG32Uint,
        RG32Sint                    = WGPUTextureFormat_RG32Sint,
        RGBA16Uint                  = WGPUTextureFormat_RGBA16Uint,
        RGBA16Sint                  = WGPUTextureFormat_RGBA16Sint,
        RGBA16Float                 = WGPUTextureFormat_RGBA16Float,
        RGBA32Float                 = WGPUTextureFormat_RGBA32Float,
        RGBA32Uint                  = WGPUTextureFormat_RGBA32Uint,
        RGBA32Sint                  = WGPUTextureFormat_RGBA32Sint,
        Stencil8                    = WGPUTextureFormat_Stencil8,
        Depth16Unorm                = WGPUTextureFormat_Depth16Unorm,
        Depth24Plus                 = WGPUTextureFormat_Depth24Plus,
        Depth24PlusStencil8         = WGPUTextureFormat_Depth24PlusStencil8,
        Depth32Float                = WGPUTextureFormat_Depth32Float,
        Depth32FloatStencil8        = WGPUTextureFormat_Depth32FloatStencil8,
        BC1RGBAUnorm                = WGPUTextureFormat_BC1RGBAUnorm,
        BC1RGBAUnormSrgb            = WGPUTextureFormat_BC1RGBAUnormSrgb,
        BC2RGBAUnorm                = WGPUTextureFormat_BC2RGBAUnorm,
        BC2RGBAUnormSrgb            = WGPUTextureFormat_BC2RGBAUnormSrgb,
        BC3RGBAUnorm                = WGPUTextureFormat_BC3RGBAUnorm,
        BC3RGBAUnormSrgb            = WGPUTextureFormat_BC3RGBAUnormSrgb,
        BC4RUnorm                   = WGPUTextureFormat_BC4RUnorm,
        BC4RSnorm                   = WGPUTextureFormat_BC4RSnorm,
        BC5RGUnorm                  = WGPUTextureFormat_BC5RGUnorm,
        BC5RGSnorm                  = WGPUTextureFormat_BC5RGSnorm,
        BC6HRGBUfloat               = WGPUTextureFormat_BC6HRGBUfloat,
        BC6HRGBFloat                = WGPUTextureFormat_BC6HRGBFloat,
        BC7RGBAUnorm                = WGPUTextureFormat_BC7RGBAUnorm,
        BC7RGBAUnormSrgb            = WGPUTextureFormat_BC7RGBAUnormSrgb,
        ETC2RGB8Unorm               = WGPUTextureFormat_ETC2RGB8Unorm,
        ETC2RGB8UnormSrgb           = WGPUTextureFormat_ETC2RGB8UnormSrgb,
        ETC2RGB8A1Unorm             = WGPUTextureFormat_ETC2RGB8A1Unorm,
        ETC2RGB8A1UnormSrgb         = WGPUTextureFormat_ETC2RGB8A1UnormSrgb,
        ETC2RGBA8Unorm              = WGPUTextureFormat_ETC2RGBA8Unorm,
        ETC2RGBA8UnormSrgb          = WGPUTextureFormat_ETC2RGBA8UnormSrgb,
        EACR11Unorm                 = WGPUTextureFormat_EACR11Unorm,
        EACR11Snorm                 = WGPUTextureFormat_EACR11Snorm,
        EACRG11Unorm                = WGPUTextureFormat_EACRG11Unorm,
        EACRG11Snorm                = WGPUTextureFormat_EACRG11Snorm,
        ASTC4x4Unorm                = WGPUTextureFormat_ASTC4x4Unorm,
        ASTC4x4UnormSrgb            = WGPUTextureFormat_ASTC4x4UnormSrgb,
        ASTC5x4Unorm                = WGPUTextureFormat_ASTC5x4Unorm,
        ASTC5x4UnormSrgb            = WGPUTextureFormat_ASTC5x4UnormSrgb,
        ASTC5x5Unorm                = WGPUTextureFormat_ASTC5x5Unorm,
        ASTC5x5UnormSrgb            = WGPUTextureFormat_ASTC5x5UnormSrgb,
        ASTC6x5Unorm                = WGPUTextureFormat_ASTC6x5Unorm,
        ASTC6x5UnormSrgb            = WGPUTextureFormat_ASTC6x5UnormSrgb,
        ASTC6x6Unorm                = WGPUTextureFormat_ASTC6x6Unorm,
        ASTC6x6UnormSrgb            = WGPUTextureFormat_ASTC6x6UnormSrgb,
        ASTC8x5Unorm                = WGPUTextureFormat_ASTC8x5Unorm,
        ASTC8x5UnormSrgb            = WGPUTextureFormat_ASTC8x5UnormSrgb,
        ASTC8x6Unorm                = WGPUTextureFormat_ASTC8x6Unorm,
        ASTC8x6UnormSrgb            = WGPUTextureFormat_ASTC8x6UnormSrgb,
        ASTC8x8Unorm                = WGPUTextureFormat_ASTC8x8Unorm,
        ASTC8x8UnormSrgb            = WGPUTextureFormat_ASTC8x8UnormSrgb,
        ASTC10x5Unorm               = WGPUTextureFormat_ASTC10x5Unorm,
        ASTC10x5UnormSrgb           = WGPUTextureFormat_ASTC10x5UnormSrgb,
        ASTC10x6Unorm               = WGPUTextureFormat_ASTC10x6Unorm,
        ASTC10x6UnormSrgb           = WGPUTextureFormat_ASTC10x6UnormSrgb,
        ASTC10x8Unorm               = WGPUTextureFormat_ASTC10x8Unorm,
        ASTC10x8UnormSrgb           = WGPUTextureFormat_ASTC10x8UnormSrgb,
        ASTC10x10Unorm              = WGPUTextureFormat_ASTC10x10Unorm,
        ASTC10x10UnormSrgb          = WGPUTextureFormat_ASTC10x10UnormSrgb,
        ASTC12x10Unorm              = WGPUTextureFormat_ASTC12x10Unorm,
        ASTC12x10UnormSrgb          = WGPUTextureFormat_ASTC12x10UnormSrgb,
        ASTC12x12Unorm              = WGPUTextureFormat_ASTC12x12Unorm,
        ASTC12x12UnormSrgb          = WGPUTextureFormat_ASTC12x12UnormSrgb,
#ifdef WEBGPU_BACKEND_DAWN
        R16Unorm                    = WGPUTextureFormat_R16Unorm,
        RG16Unorm                   = WGPUTextureFormat_RG16Unorm,
        RGBA16Unorm                 = WGPUTextureFormat_RGBA16Unorm,
        R16Snorm                    = WGPUTextureFormat_R16Snorm,
        RG16Snorm                   = WGPUTextureFormat_RG16Snorm,
        RGBA16Snorm                 = WGPUTextureFormat_RGBA16Snorm,
        R8BG8Biplanar420Unorm       = WGPUTextureFormat_R8BG8Biplanar420Unorm,
        R10X6BG10X6Biplanar420Unorm = WGPUTextureFormat_R10X6BG10X6Biplanar420Unorm,
        R8BG8A8Triplanar420Unorm    = WGPUTextureFormat_R8BG8A8Triplanar420Unorm,
        R8BG8Biplanar422Unorm       = WGPUTextureFormat_R8BG8Biplanar422Unorm,
        R8BG8Biplanar444Unorm       = WGPUTextureFormat_R8BG8Biplanar444Unorm,
        R10X6BG10X6Biplanar422Unorm = WGPUTextureFormat_R10X6BG10X6Biplanar422Unorm,
        R10X6BG10X6Biplanar444Unorm = WGPUTextureFormat_R10X6BG10X6Biplanar444Unorm,
        External                    = WGPUTextureFormat_External,
#endif
    };

    enum class TextureSampleType : uint32_t
    {
        Undefined         = WGPUTextureSampleType_Undefined,
        Float             = WGPUTextureSampleType_Float,
        UnfilterableFloat = WGPUTextureSampleType_UnfilterableFloat,
        Depth             = WGPUTextureSampleType_Depth,
        Sint              = WGPUTextureSampleType_Sint,
        Uint              = WGPUTextureSampleType_Uint,
    };

    enum class TextureUsageFlags : uint32_t
    {
        None                = WGPUTextureUsage_None,
        CopySrc             = WGPUTextureUsage_CopySrc,
        CopyDst             = WGPUTextureUsage_CopyDst,
        TextureBinding      = WGPUTextureUsage_TextureBinding,
        StorageBinding      = WGPUTextureUsage_StorageBinding,
        RenderAttachment    = WGPUTextureUsage_RenderAttachment,
#ifdef WEBGPU_BACKEND_DAWN
        TransientAttachment = WGPUTextureUsage_TransientAttachment,
        StorageAttachment   = WGPUTextureUsage_StorageAttachment,
#endif
    };

    enum class TextureViewDimension : uint32_t
    {
        Undefined = WGPUTextureViewDimension_Undefined,
        _1D       = WGPUTextureViewDimension_1D,
        _2D       = WGPUTextureViewDimension_2D,
        _2DArray  = WGPUTextureViewDimension_2DArray,
        Cube      = WGPUTextureViewDimension_Cube,
        CubeArray = WGPUTextureViewDimension_CubeArray,
        _3D       = WGPUTextureViewDimension_3D,
    };

    TextureUsageFlags operator|(TextureUsageFlags lhs, TextureUsageFlags rhs);
    TextureUsageFlags & operator|=(TextureUsageFlags& lhs, TextureUsageFlags rhs);
    TextureUsageFlags operator&(TextureUsageFlags lhs, TextureUsageFlags rhs);
    TextureUsageFlags & operator&=(TextureUsageFlags& lhs, TextureUsageFlags rhs);
    bool operator==(TextureUsageFlags lhs, TextureUsageFlags rhs);

    enum class VertexFormat : uint32_t
    {
        Undefined       = WGPUVertexFormat_Undefined,
        Uint8x2         = WGPUVertexFormat_Uint8x2,
        Uint8x4         = WGPUVertexFormat_Uint8x4,
        Sint8x2         = WGPUVertexFormat_Sint8x2,
        Sint8x4         = WGPUVertexFormat_Sint8x4,
        Unorm8x2        = WGPUVertexFormat_Unorm8x2,
        Unorm8x4        = WGPUVertexFormat_Unorm8x4,
        Snorm8x2        = WGPUVertexFormat_Snorm8x2,
        Snorm8x4        = WGPUVertexFormat_Snorm8x4,
        Uint16x2        = WGPUVertexFormat_Uint16x2,
        Uint16x4        = WGPUVertexFormat_Uint16x4,
        Sint16x2        = WGPUVertexFormat_Sint16x2,
        Sint16x4        = WGPUVertexFormat_Sint16x4,
        Unorm16x2       = WGPUVertexFormat_Unorm16x2,
        Unorm16x4       = WGPUVertexFormat_Unorm16x4,
        Snorm16x2       = WGPUVertexFormat_Snorm16x2,
        Snorm16x4       = WGPUVertexFormat_Snorm16x4,
        Float16x2       = WGPUVertexFormat_Float16x2,
        Float16x4       = WGPUVertexFormat_Float16x4,
        Float32         = WGPUVertexFormat_Float32,
        Float32x2       = WGPUVertexFormat_Float32x2,
        Float32x3       = WGPUVertexFormat_Float32x3,
        Float32x4       = WGPUVertexFormat_Float32x4,
        Uint32          = WGPUVertexFormat_Uint32,
        Uint32x2        = WGPUVertexFormat_Uint32x2,
        Uint32x3        = WGPUVertexFormat_Uint32x3,
        Uint32x4        = WGPUVertexFormat_Uint32x4,
        Sint32          = WGPUVertexFormat_Sint32,
        Sint32x2        = WGPUVertexFormat_Sint32x2,
        Sint32x3        = WGPUVertexFormat_Sint32x3,
        Sint32x4        = WGPUVertexFormat_Sint32x4,
#ifdef WEBGPU_BACKEND_DAWN
        Unorm10_10_10_2 = WGPUVertexFormat_Unorm10_10_10_2,
#endif
    };

    enum class VertexStepMode : uint32_t
    {
#ifdef WEBGPU_BACKEND_DAWN
        Undefined = WGPUVertexStepMode_Undefined,
#endif
        VertexBufferNotUsed = WGPUVertexStepMode_VertexBufferNotUsed,
        Vertex              = WGPUVertexStepMode_Vertex,
        Instance            = WGPUVertexStepMode_Instance,
    };

    // Callback Types
    using MapBufferCallback = std::function<void(BufferMapAsyncStatus status)>;
    using RequestAdapterCallback = std::function<void(RequestAdapterStatus status, const Adapter &adapter,
        const std::string &message)>;
    using RequestDeviceCallback = std::function<void(RequestDeviceStatus status, const Device &device,
        const std::string &message)>;

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

        [[nodiscard]] WGPUAdapter c_ptr() const;

        [[nodiscard]] std::expected<Device, std::string> create_device(const DeviceDescriptor &descriptor) const;
        [[nodiscard]] std::vector<FeatureName> enumerate_features() const;
        [[nodiscard]] std::optional<SupportedLimits> get_limits() const;
        [[nodiscard]] std::optional<AdapterProperties> get_properties() const;
        [[nodiscard]] bool has_feature(FeatureName feature) const;
        [[nodiscard]] std::unique_ptr<RequestDeviceCallback> request_device(const DeviceDescriptor &descriptor,
            RequestDeviceCallback &&callback) const;

    private:
        WGPUAdapter m_handle{nullptr};
    };

    class BindGroup
    {
    public:
        explicit BindGroup(const WGPUBindGroup &handle);
        ~BindGroup();

        BindGroup(const BindGroup &other);
        BindGroup(BindGroup &&other) noexcept;
        BindGroup & operator=(const BindGroup &other);
        BindGroup & operator=(BindGroup &&other) noexcept;

        [[nodiscard]] WGPUBindGroup c_ptr() const;

    private:
        WGPUBindGroup m_handle{nullptr};
    };

    class BindGroupLayout
    {
    public:
        explicit BindGroupLayout(const WGPUBindGroupLayout &handle);
        ~BindGroupLayout();

        BindGroupLayout(const BindGroupLayout &other);
        BindGroupLayout(BindGroupLayout &&other) noexcept;
        BindGroupLayout & operator=(const BindGroupLayout &other);
        BindGroupLayout & operator=(BindGroupLayout &&other) noexcept;

        [[nodiscard]] WGPUBindGroupLayout c_ptr() const;

    private:
        WGPUBindGroupLayout m_handle{nullptr};
    };

    class Buffer
    {
    public:
        explicit Buffer(const WGPUBuffer &handle);
        ~Buffer();

        Buffer(const Buffer &other);
        Buffer(Buffer &&other) noexcept;
        Buffer & operator=(const Buffer &other);
        Buffer & operator=(Buffer &&other) noexcept;

        [[nodiscard]] WGPUBuffer c_ptr() const;

        [[nodiscard]] const void * get_const_mapped_range(size_t offset, size_t size) const;
        template<typename T>
        [[nodiscard]] const T * get_const_mapped_range(size_t offset, size_t count) const;
        [[nodiscard]] uint64_t get_size() const;
        [[nodiscard]] std::unique_ptr<MapBufferCallback> map_async(MapModeFlags mode, size_t offset, size_t size,
            MapBufferCallback &&callback) const;
        void unmap() const;

    private:
        WGPUBuffer m_handle{nullptr};
    };

    class CommandBuffer
    {
    public:
        explicit CommandBuffer(const WGPUCommandBuffer &handle);
        ~CommandBuffer();

        CommandBuffer(const CommandBuffer &other);
        CommandBuffer(CommandBuffer &&other) noexcept;
        CommandBuffer & operator=(const CommandBuffer &other);
        CommandBuffer & operator=(CommandBuffer &&other) noexcept;

        [[nodiscard]] WGPUCommandBuffer c_ptr() const;

    private:
        WGPUCommandBuffer m_handle{nullptr};
    };

    class CommandEncoder
    {
    public:
        explicit CommandEncoder(const WGPUCommandEncoder &handle);
        ~CommandEncoder();

        CommandEncoder(const CommandEncoder &other);
        CommandEncoder(CommandEncoder &&other) noexcept;
        CommandEncoder & operator=(const CommandEncoder &other);
        CommandEncoder & operator=(CommandEncoder &&other) noexcept;

        [[nodiscard]] WGPUCommandEncoder c_ptr() const;

        [[nodiscard]] RenderPassEncoder begin_render_pass(const RenderPassDescriptor &descriptor) const;
        void copy_buffer_to_buffer(const Buffer &source, uint64_t source_offset, const Buffer &destination,
            uint64_t destination_offset, uint64_t size) const;
        [[nodiscard]] CommandBuffer finish(const CommandBufferDescriptor &descriptor) const;

    private:
        WGPUCommandEncoder m_handle{nullptr};
    };

    class Device
    {
    public:
        explicit Device(const WGPUDevice &handle);
        ~Device();

        Device(const Device &other);
        Device(Device &&other) noexcept;
        Device & operator=(const Device &other);
        Device & operator=(Device &&other) noexcept;

        [[nodiscard]] WGPUDevice c_ptr() const;

        [[nodiscard]] BindGroup create_bind_group(const BindGroupDescriptor &descriptor) const;
        [[nodiscard]] BindGroupLayout create_bind_group_layout(const BindGroupLayoutDescriptor &descriptor) const;
        [[nodiscard]] Buffer create_buffer(const BufferDescriptor &descriptor) const;
        [[nodiscard]] CommandEncoder create_command_encoder(const CommandEncoderDescriptor &descriptor) const;
        [[nodiscard]] PipelineLayout create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) const;
        [[nodiscard]] RenderPipeline create_render_pipeline(const RenderPipelineDescriptor &descriptor) const;
        [[nodiscard]] Sampler create_sampler(const SamplerDescriptor &descriptor) const;
        [[nodiscard]] ShaderModule create_shader_module(const ShaderModuleDescriptor &descriptor) const;
        [[nodiscard]] Texture create_texture(const TextureDescriptor &descriptor) const;
        [[nodiscard]] std::optional<SupportedLimits> get_limits() const;
        [[nodiscard]] Queue get_queue() const;
        void tick() const;

    private:
        WGPUDevice m_handle{nullptr};
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

        [[nodiscard]] WGPUInstance c_ptr() const;

        [[nodiscard]] std::expected<Adapter, std::string> create_adapter(const RequestAdapterOptions &options) const;
        void process_events() const;
        [[nodiscard]] std::unique_ptr<RequestAdapterCallback> request_adapter(const RequestAdapterOptions &options,
            RequestAdapterCallback &&callback) const;

    private:
        WGPUInstance m_handle{nullptr};
    };

    class PipelineLayout
    {
    public:
        explicit PipelineLayout(const WGPUPipelineLayout &handle);
        ~PipelineLayout();

        PipelineLayout(const PipelineLayout &other);
        PipelineLayout(PipelineLayout &&other) noexcept;
        PipelineLayout & operator=(const PipelineLayout &other);
        PipelineLayout & operator=(PipelineLayout &&other) noexcept;

        [[nodiscard]] WGPUPipelineLayout c_ptr() const;

    private:
        WGPUPipelineLayout m_handle{nullptr};
    };

    class Queue
    {
    public:
        explicit Queue(const WGPUQueue &handle);
        ~Queue();

        Queue(const Queue &other);
        Queue(Queue &&other) noexcept;
        Queue & operator=(const Queue &other);
        Queue & operator=(Queue &&other) noexcept;

        [[nodiscard]] WGPUQueue c_ptr() const;

        void submit(const std::vector<CommandBuffer> &commands) const;
        template<typename T>
        void write_buffer(const Buffer &buffer, uint64_t buffer_offset, const T &data) const;
        template<typename T>
        void write_buffer(const Buffer &buffer, uint64_t buffer_offset, const std::vector<T> &data) const;
        template<typename T>
        void write_texture(const ImageCopyTexture &destination, const std::vector<T> &data, const TextureDataLayout &data_layout, const Extent3D &write_size) const;


    private:
        WGPUQueue m_handle{nullptr};
    };

    class RenderPassEncoder
    {
    public:
        explicit RenderPassEncoder(const WGPURenderPassEncoder &handle);
        ~RenderPassEncoder();

        RenderPassEncoder(const RenderPassEncoder &other);
        RenderPassEncoder(RenderPassEncoder &&other) noexcept;
        RenderPassEncoder & operator=(const RenderPassEncoder &other);
        RenderPassEncoder & operator=(RenderPassEncoder &&other) noexcept;

        [[nodiscard]] WGPURenderPassEncoder c_ptr() const;

        void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const;
        void draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t base_vertex,
            uint32_t first_instance) const;
        void end() const;
        void set_bind_group(uint32_t group_index, const BindGroup &group,
            const std::vector<uint32_t> &dynamic_offsets = {}) const;
        void set_index_buffer(const Buffer &buffer, IndexFormat format, uint64_t offset, uint64_t size) const;
        void set_pipeline(const RenderPipeline &pipeline) const;
        void set_vertex_buffer(uint32_t slot, const Buffer &buffer, uint64_t offset, uint64_t size) const;

    private:
        WGPURenderPassEncoder m_handle{nullptr};
    };

    class RenderPipeline
    {
    public:
        explicit RenderPipeline(const WGPURenderPipeline &handle);
        ~RenderPipeline();

        RenderPipeline(const RenderPipeline &other);
        RenderPipeline(RenderPipeline &&other) noexcept;
        RenderPipeline & operator=(const RenderPipeline &other);
        RenderPipeline & operator=(RenderPipeline &&other) noexcept;

        [[nodiscard]] WGPURenderPipeline c_ptr() const;

    private:
        WGPURenderPipeline m_handle{nullptr};
    };

    class Sampler
    {
    public:
        explicit Sampler(const WGPUSampler &handle);
        ~Sampler();

        Sampler(const Sampler &other);
        Sampler(Sampler &&other) noexcept;
        Sampler & operator=(const Sampler &other);
        Sampler & operator=(Sampler &&other) noexcept;

        [[nodiscard]] WGPUSampler c_ptr() const;

    private:
        WGPUSampler m_handle{nullptr};
    };

    class ShaderModule
    {
    public:
        explicit ShaderModule(const WGPUShaderModule &handle);
        ~ShaderModule();

        ShaderModule(const ShaderModule &other);
        ShaderModule(ShaderModule &&other) noexcept;
        ShaderModule & operator=(const ShaderModule &other);
        ShaderModule & operator=(ShaderModule &&other) noexcept;

        [[nodiscard]] WGPUShaderModule c_ptr() const;

    private:
        WGPUShaderModule m_handle{nullptr};
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

        [[nodiscard]] WGPUSurface c_ptr() const;

        void configure(const SurfaceConfiguration &configuration) const;
        [[nodiscard]] SurfaceCapabilities get_capabilities(const Adapter &adapter) const;
        [[nodiscard]] SurfaceTexture get_current_texture() const;
        void present() const;
        void unconfigure() const;

    private:
        WGPUSurface m_handle{nullptr};
    };

    class Texture
    {
    public:
        explicit Texture(const WGPUTexture &handle);
        ~Texture();

        Texture(const Texture &other);
        Texture(Texture &&other) noexcept;
        Texture & operator=(const Texture &other);
        Texture & operator=(Texture &&other) noexcept;

        [[nodiscard]] WGPUTexture c_ptr() const;

        [[nodiscard]] TextureView create_view() const;
        [[nodiscard]] TextureView create_view(const TextureViewDescriptor &descriptor) const;
        [[nodiscard]] uint32_t get_depth_or_array_layers() const;
        [[nodiscard]] TextureDimension get_dimension() const;
        [[nodiscard]] TextureFormat get_format() const;
        [[nodiscard]] uint32_t get_height() const;
        [[nodiscard]] uint32_t get_mip_level_count() const;
        [[nodiscard]] uint32_t get_sample_count() const;
        [[nodiscard]] TextureUsageFlags get_usage() const;
        [[nodiscard]] uint32_t get_width() const;

    private:
        WGPUTexture m_handle{nullptr};
    };

    class TextureView
    {
    public:
        explicit TextureView(const WGPUTexture &texture, const WGPUTextureView &handle);
        ~TextureView();

        TextureView(const TextureView &other);
        TextureView(TextureView &&other) noexcept;
        TextureView & operator=(const TextureView &other);
        TextureView & operator=(TextureView &&other) noexcept;

        [[nodiscard]] WGPUTextureView c_ptr() const;

    private:
        // On WGPU, WGPUTextureView needs its WGPUTexture to stick around. Otherwise it fails.
        WGPUTexture m_texture{nullptr};
        WGPUTextureView m_handle{nullptr};
    };

    // Structs
    struct AdapterProperties
    {
        ChainedStructOut *next_in_chain;
        uint32_t vendor_id;
        std::string vendor_name;
        std::string architecture;
        uint32_t device_id;
        std::string name;
        std::string driver_description;
        AdapterType adapter_type;
        BackendType backend_type;
    };

    struct BindGroupDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        BindGroupLayout layout;
        std::vector<BindGroupEntry> entries;
    };

    struct BindGroupEntry
    {
        const ChainedStruct *next_in_chain;
        uint32_t binding;
        std::optional<Buffer> buffer;
        uint64_t offset;
        uint64_t size;
        std::optional<Sampler> sampler;
        std::optional<TextureView> texture_view;
    };

    struct BindGroupLayoutDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        std::vector<BindGroupLayoutEntry> entries;
    };

    struct BufferBindingLayout
    {
        const ChainedStruct *next_in_chain;
        BufferBindingType type;
        bool has_dynamic_offset;
        uint64_t min_binding_size;
    };

    struct BufferDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        BufferUsageFlags usage;
        uint64_t size;
        bool mapped_at_creation;
    };

    struct ChainedStruct
    {
        const ChainedStruct *next_in_chain;
        SType s_type;
    };

    struct ChainedStructOut
    {
        ChainedStructOut *next_in_chain;
        SType s_type;
    };

    struct Color
    {
        double r;
        double g;
        double b;
        double a;
    };

    struct CommandBufferDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
    };

    struct CommandEncoderDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
    };

    struct ConstantEntry
    {
        const ChainedStruct *next_in_chain;
        std::string key;
        double value;
    };

    struct QueueDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
    };

    struct DeviceDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        std::vector<FeatureName> required_features;
        WGPU_NULLABLE const RequiredLimits *required_limits;
        QueueDescriptor default_queue;
    };

    struct Extent3D
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth_or_array_layers;
    };

    struct FragmentState
    {
        const ChainedStruct *next_in_chain;
        ShaderModule module;
        std::optional<std::string> entry_point;
        std::vector<ConstantEntry> constants;
        std::vector<ColorTargetState> targets;
    };

#ifdef WEBGPU_BACKEND_DAWN
    struct InstanceFeatures
    {
        const ChainedStruct *next_in_chain;
        bool timed_wait_any_enable;
        size_t timed_wait_any_max_count;
    };
#endif

    struct InstanceDescriptor
    {
        const ChainedStruct *next_in_chain;
#ifdef WEBGPU_BACKEND_DAWN
        InstanceFeatures features;
#endif
    };

    struct Limits
    {
        uint32_t max_texture_dimension_1d;
        uint32_t max_texture_dimension_2d;
        uint32_t max_texture_dimension_3d;
        uint32_t max_texture_array_layers;
        uint32_t max_bind_groups;
        uint32_t max_bind_groups_plus_vertex_buffers;
        uint32_t max_bindings_per_bind_group;
        uint32_t max_dynamic_uniform_buffers_per_pipeline_layout;
        uint32_t max_dynamic_storage_buffers_per_pipeline_layout;
        uint32_t max_sampled_textures_per_shader_stage;
        uint32_t max_samplers_per_shader_stage;
        uint32_t max_storage_buffers_per_shader_stage;
        uint32_t max_storage_textures_per_shader_stage;
        uint32_t max_uniform_buffers_per_shader_stage;
        uint64_t max_uniform_buffer_binding_size;
        uint64_t max_storage_buffer_binding_size;
        uint32_t min_uniform_buffer_offset_alignment;
        uint32_t min_storage_buffer_offset_alignment;
        uint32_t max_vertex_buffers;
        uint64_t max_buffer_size;
        uint32_t max_vertex_attributes;
        uint32_t max_vertex_buffer_array_stride;
        uint32_t max_inter_stage_shader_components;
        uint32_t max_inter_stage_shader_variables;
        uint32_t max_color_attachments;
        uint32_t max_color_attachment_bytes_per_sample;
        uint32_t max_compute_workgroup_storage_size;
        uint32_t max_compute_invocations_per_workgroup;
        uint32_t max_compute_workgroup_size_x;
        uint32_t max_compute_workgroup_size_y;
        uint32_t max_compute_workgroup_size_z;
        uint32_t max_compute_workgroups_per_dimension;
    };

    struct MultisampleState
    {
        const ChainedStruct *next_in_chain;
        uint32_t count;
        uint32_t mask;
        bool alpha_to_coverage_enabled;
    };

    struct Origin3D
    {
        uint32_t x;
        uint32_t y;
        uint32_t z;
    };

    struct PipelineLayoutDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        std::vector<BindGroupLayout> bind_group_layouts;
    };

    struct PrimitiveState
    {
        const ChainedStruct *next_in_chain;
        PrimitiveTopology topology;
        IndexFormat strip_index_format;
        FrontFace front_face;
        CullMode cull_mode;
    };

    struct RequestAdapterOptions
    {
        const ChainedStruct *next_in_chain;
        WGPU_NULLABLE const Surface *compatible_surface;
        PowerPreference power_preference;
        BackendType backend_type;
        bool force_fallback_adapter;
#ifdef WEBGPU_BACKEND_DAWN
        bool compatibility_mode;
#endif
    };

    struct RequiredLimits
    {
        const ChainedStruct *next_in_chain;
        Limits limits;
    };

    struct RenderPassColorAttachment
    {
        const ChainedStruct *next_in_chain;
        std::optional<TextureView> view;
#ifdef WEBGPU_BACKEND_DAWN
        uint32_t depth_slice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif
        std::optional<TextureView> resolve_target;
        LoadOp load_op;
        StoreOp store_op;
        Color clear_value;
    };

    struct RenderPassDepthStencilAttachment
    {
        TextureView view;
        LoadOp depth_load_op;
        StoreOp depth_store_op;
        float depth_clear_value;
        bool depth_read_only;
        LoadOp stencil_load_op;
        StoreOp stencil_store_op;
        uint32_t stencil_clear_value;
        bool stencil_read_only;
    };

    struct RenderPassDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        std::vector<RenderPassColorAttachment> color_attachments;
        std::optional<RenderPassDepthStencilAttachment> depth_stencil_attachment;
        // TODO: QuerySet
        // TODO: RenderPassTimestampWrites
    };

    struct SamplerBindingLayout
    {
        const ChainedStruct *next_in_chain;
        SamplerBindingType type;
    };

    struct SamplerDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        AddressMode address_mode_u;
        AddressMode address_mode_v;
        AddressMode address_mode_w;
        FilterMode mag_filter;
        FilterMode min_filter;
        MipmapFilterMode mipmap_filter;
        float lod_min_clamp;
        float lod_max_clamp;
        CompareFunction compare;
        uint16_t max_anistropy;
    };

    struct ShaderModuleDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
    };

    struct ShaderModuleSPIRVDescriptor
    {
        ChainedStruct chain;
        uint32_t code_size;
        const uint32_t *code;
    };

    struct ShaderModuleWGSLDescriptor
    {
        ChainedStruct chain;
        const char *code;
    };

    struct StencilFaceState
    {
        CompareFunction compare;
        StencilOperation fail_op;
        StencilOperation depth_fail_op;
        StencilOperation pass_op;
    };

    struct StorageTextureBindingLayout
    {
        const ChainedStruct *next_in_chain;
        StorageTextureAccess access;
        TextureFormat format;
        TextureViewDimension view_dimension;
    };

    struct SupportedLimits
    {
        ChainedStructOut *next_in_chain;
        Limits limits;
    };

    struct SurfaceCapabilities
    {
        ChainedStructOut *next_in_chain;
#ifdef WEBGPU_BACKEND_DAWN
        TextureUsageFlags usages;
#endif
        std::vector<TextureFormat> formats;
        std::vector<PresentMode> present_modes;
        std::vector<CompositeAlphaMode> alpha_modes;
    };

    struct SurfaceConfiguration
    {
        const ChainedStruct *next_in_chain;
        Device device;
        TextureFormat format;
        TextureUsageFlags usage;
        std::vector<TextureFormat> view_formats;
        CompositeAlphaMode alpha_mode;
        uint32_t width;
        uint32_t height;
        PresentMode present_mode;
    };

    struct SurfaceTexture
    {
        Texture texture;
        bool suboptimal;
        SurfaceGetCurrentTextureStatus status;
    };

    struct TextureBindingLayout
    {
        const ChainedStruct *next_in_chain;
        TextureSampleType sample_type;
        TextureViewDimension view_dimension;
        bool multisampled;
    };

    struct TextureDataLayout
    {
        const ChainedStruct *next_in_chain;
        uint64_t offset;
        uint32_t bytes_per_row;
        uint32_t rows_per_image;
    };

    struct TextureDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        TextureUsageFlags usage;
        TextureDimension dimension;
        Extent3D size;
        TextureFormat format;
        uint32_t mip_level_count;
        uint32_t sample_count;
        std::vector<TextureFormat> view_formats;
    };

    struct TextureViewDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        TextureFormat format;
        TextureViewDimension dimension;
        uint32_t base_mip_level;
        uint32_t mip_level_count;
        uint32_t base_array_layer;
        uint32_t array_layer_count;
        TextureAspect aspect;
    };

    struct VertexAttribute
    {
        VertexFormat format;
        uint64_t offset;
        uint32_t shader_location;
    };

    struct VertexBufferLayout
    {
        uint64_t array_stride;
        VertexStepMode step_mode;
        std::vector<VertexAttribute> attributes;
    };

    struct VertexState
    {
        const ChainedStruct *next_in_chain;
        ShaderModule module;
        std::optional<std::string> entry_point;
        std::vector<ConstantEntry> constants;
        std::vector<VertexBufferLayout> buffers;
    };

    struct BindGroupLayoutEntry
    {
        const ChainedStruct *next_in_chain;
        uint32_t binding;
        ShaderStageFlags visibility;
        BufferBindingLayout buffer;
        SamplerBindingLayout sampler;
        TextureBindingLayout texture;
        StorageTextureBindingLayout storage_texture;
    };

    struct BlendComponent
    {
        BlendOperation operation;
        BlendFactor src_factor;
        BlendFactor dst_factor;
    };

    struct BlendState
    {
        BlendComponent color;
        BlendComponent alpha;
    };

    struct ColorTargetState
    {
        const ChainedStruct *next_in_chain;
        TextureFormat format;
        std::optional<BlendState> blend;
        ColorWriteMaskFlags write_mask;
    };

    struct DepthStencilState
    {
        const ChainedStruct *next_in_chain;
        TextureFormat format;
        bool depth_write_enabled;
        CompareFunction depth_compare;
        StencilFaceState stencil_front;
        StencilFaceState stencil_back;
        uint32_t stencil_read_mask;
        uint32_t stencil_write_mask;
        int32_t depth_bias;
        float depth_bias_slope_scale;
        float depth_bias_clamp;
    };

    struct ImageCopyTexture
    {
#ifdef WEBGPU_BACKEND_WGPU
        const ChainedStruct *next_in_chain;
#endif
        Texture texture;
        uint32_t mip_level;
        Origin3D origin;
        TextureAspect aspect;
    };

    struct RenderPipelineDescriptor
    {
        const ChainedStruct *next_in_chain;
        std::string label;
        std::optional<PipelineLayout> layout;
        VertexState vertex;
        PrimitiveState primitive;
        std::optional<DepthStencilState> depth_stencil;
        MultisampleState multisample;
        std::optional<FragmentState> fragment;
    };

    // Non-member Functions
    Instance create_instance(const InstanceDescriptor &descriptor);

    // Template Definitions
    template<typename T>
    [[nodiscard]] const T * Buffer::get_const_mapped_range(const size_t offset, const size_t count) const
    {
        return static_cast<const T *>(get_const_mapped_range(offset, count * sizeof(T)));
    }

    template<typename T>
    void Queue::write_buffer(const Buffer &buffer, const uint64_t buffer_offset, const T &data) const
    {
        wgpuQueueWriteBuffer(m_handle, buffer.c_ptr(), buffer_offset, &data, sizeof(T));
    }

    template<typename T>
    void Queue::write_buffer(const Buffer &buffer, const uint64_t buffer_offset, const std::vector<T> &data) const
    {
        wgpuQueueWriteBuffer(m_handle, buffer.c_ptr(), buffer_offset, data.data(), data.size() * sizeof(T));
    }

    template<typename T>
    void Queue::write_texture(const ImageCopyTexture &destination, const std::vector<T> &data,
        const TextureDataLayout &data_layout, const Extent3D &write_size) const
    {
        const WGPUImageCopyTexture wgpu_destination
        {
#ifdef WEBGPU_BACKEND_WGPU
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(destination.next_in_chain),
#endif
            .texture = destination.texture.c_ptr(),
            .mipLevel = destination.mip_level,
            .origin = {destination.origin.x, destination.origin.y, destination.origin.z},
            .aspect = static_cast<WGPUTextureAspect>(destination.aspect)
        };

        const WGPUTextureDataLayout wgpu_data_layout
        {
            .nextInChain = reinterpret_cast<const WGPUChainedStruct *>(data_layout.next_in_chain),
            .offset = data_layout.offset,
            .bytesPerRow = data_layout.bytes_per_row,
            .rowsPerImage = data_layout.rows_per_image
        };

        const WGPUExtent3D wgpu_write_size
        {
            .width = write_size.width,
            .height = write_size.height,
            .depthOrArrayLayers = write_size.depth_or_array_layers
        };

        wgpuQueueWriteTexture(m_handle, &wgpu_destination, data.data(), data.size(), &wgpu_data_layout,
            &wgpu_write_size);
    }
}
