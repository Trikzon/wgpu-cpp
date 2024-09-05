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
    enum class AdapterType : uint32_t
    {
        DiscreteGPU   = WGPUAdapterType_DiscreteGPU,
        IntegratedGPU = WGPUAdapterType_IntegratedGPU,
        CPU           = WGPUAdapterType_CPU,
        Unknown       = WGPUAdapterType_Unknown,
        Force32       = WGPUAdapterType_Force32,
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
#if WEBGPU_BACKEND_DAWN
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
        Force32 = WGPUFeatureName_Force32,
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
        Success         = WGPURequestAdapterStatus_Success,
#if WEBGPU_BACKEND_DAWN
        InstanceDropped = WGPURequestAdapterStatus_InstanceDropped,
#endif
        Unavailable     = WGPURequestAdapterStatus_Unavailable,
        Error           = WGPURequestAdapterStatus_Error,
        Unknown         = WGPURequestAdapterStatus_Unknown,
        Force32         = WGPURequestAdapterStatus_Force32,
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
        Force32                                            = WGPUSType_Force32,
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
#if WEBGPU_BACKEND_DAWN
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
        Force32                     = WGPUTextureFormat_Force32,
    };

    enum class TextureUsageFlags : uint32_t
    {
        None                = WGPUTextureUsage_None,
        CopySrc             = WGPUTextureUsage_CopySrc,
        CopyDst             = WGPUTextureUsage_CopyDst,
        TextureBinding      = WGPUTextureUsage_TextureBinding,
        StorageBinding      = WGPUTextureUsage_StorageBinding,
        RenderAttachment    = WGPUTextureUsage_RenderAttachment,
#if WEBGPU_BACKEND_DAWN
        TransientAttachment = WGPUTextureUsage_TransientAttachment,
        StorageAttachment   = WGPUTextureUsage_StorageAttachment,
#endif
        Force32             = WGPUTextureUsage_Force32,
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

    struct ChainedStructOut
    {
        ChainedStructOut *next_in_chain;
        SType s_type;
    };

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

    struct SupportedLimits
    {
        ChainedStructOut * next_in_chain;
        Limits limits;
    };

    struct SurfaceConfiguration
    {
        const ChainedStruct *next_in_chain;
        // TODO: Device device
        WGPUDevice device;
        TextureFormat format;
        TextureUsageFlags usage;
        std::vector<TextureFormat> view_formats;
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

        [[nodiscard]] std::vector<FeatureName> enumerate_features() const;
        [[nodiscard]] std::optional<SupportedLimits> get_limits() const;
        [[nodiscard]] std::optional<AdapterProperties> get_properties() const;
        [[nodiscard]] bool has_feature(FeatureName feature) const;
        // TODO: request_device

        [[nodiscard]] WGPUAdapter c_ptr() const;

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

        void process_events() const;

        [[nodiscard]] std::expected<Adapter, const char *> create_adapter(const RequestAdapterOptions &options) const;
        std::unique_ptr<RequestAdapterCallback> request_adapter(const RequestAdapterOptions &options,
            RequestAdapterCallback &&callback) const;

        [[nodiscard]] WGPUInstance c_ptr() const;

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

        void configure(const SurfaceConfiguration &configuration) const;

        [[nodiscard]] WGPUSurface c_ptr() const;

    private:
        WGPUSurface m_handle{nullptr};
    };

    // Non-member Functions
    Instance create_instance(const InstanceDescriptor &descriptor);
}
