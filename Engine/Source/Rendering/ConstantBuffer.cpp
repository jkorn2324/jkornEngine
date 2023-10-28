#include "EnginePCH.h"
#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "Profiler.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11ConstantBuffer.h"
#endif

namespace Engine
{
	ConstantBuffer::ConstantBuffer(const void* buffer, size_t stride) { }

	void ConstantBuffer::SetData(const void* bufferData, size_t stride)
	{
		SetData((void*)bufferData, stride);
	}

	bool ConstantBuffer::Create(ConstantBuffer** outConstantBuffer, const void* buffer, std::size_t stride)
	{
		PROFILE_SCOPE(CreateConstantBuffer, Rendering);

#if defined(GRAPHICS_API_DIRECTX11)
		*outConstantBuffer = new DirectX11ConstantBuffer(buffer, stride);
		return true;			
#else
		JKORN_ENGINE_ASSERT(false, "Invalid Rendering API for Constant buffer.");
		return false;
#endif
	}

	bool ConstantBuffer::Create(std::shared_ptr<ConstantBuffer>& outConstantBuffer, const void* buffer, std::size_t stride)
	{
		PROFILE_SCOPE(CreateConstantBuffer, Rendering);

#if defined(GRAPHICS_API_DIRECTX11)
		outConstantBuffer = std::make_shared<DirectX11ConstantBuffer>(buffer, stride);
		return true;			
#else
		JKORN_ENGINE_ASSERT(false, "Invalid Rendering API for Constant buffer.");
		return false;
#endif
	}
}
