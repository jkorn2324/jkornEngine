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

@protocol MTLCommandBuffer;
typedef id<MTLCommandBuffer> MTLCommandbufferPtr;

#else

using MTLDevicePtr = void*;
using CAMetalLayerPtr = void*;
using MTLCommandQueuePtr = void*;
using MTLRenderPassDescriptorPtr = void*;
using MTLCommandBufferPtr = void*;

#endif
