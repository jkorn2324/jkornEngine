#pragma once

#ifdef __OBJC__

@class CAMetalLayer;
typedef CAMetalLayer* CAMetalLayerPtr;

@protocol MTLDevice;
typedef id<MTLDevice> MTLDevicePtr;

@protocol MTLCommandQueue;
typedef id<MTLCommandQueue> MTLCommandQueuePtr;

@class MTLRenderPassDescriptor;
typedef MTLRenderPassDescriptor* MTLRenderPassDescriptorPtr;

@protocol MTLRenderCommandEncoder;
typedef id<MTLRenderCommandEncoder> MTLRenderCommandEncoderPtr;

@protocol MTLCommandBuffer;
typedef id<MTLCommandBuffer> MTLCommandBufferPtr;

@protocol MTLBuffer;
typedef id<MTLBuffer> MTLBufferPtr;

@protocol MTLTexture;
typedef id<MTLTexture> MTLTexturePtr;

#else

using MTLDevicePtr = void*;
using CAMetalLayerPtr = void*;
using MTLCommandQueuePtr = void*;
using MTLRenderPassDescriptorPtr = void*;
using MTLCommandBufferPtr = void*;
using MTLBufferPtr = void*;
using MTLRenderCommandEncoderPtr = void*;
using MTLTexturePtr = void*;

#endif
