#include "EnginePCH.h"
#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "DirectX11ConstantBuffer.h"
#include "Profiler.h"

namespace Engine
{
	ConstantBuffer::ConstantBuffer(const void* buffer, std::size_t stride) { }

	ConstantBuffer* ConstantBuffer::Create(const void* buffer, std::size_t stride)
	{
		PROFILE_SCOPE(CreateConstantBuffer, Rendering);

		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11ConstantBuffer(buffer, stride);
		case RenderingAPIType::NONE:
		{
			DebugAssert(false, "Invalid constant buffer type.");
			return nullptr;
		}
		}
		return nullptr;
	}
}