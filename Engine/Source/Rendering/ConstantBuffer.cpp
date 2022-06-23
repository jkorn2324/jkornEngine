#include "EnginePCH.h"
#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "DirectX11ConstantBuffer.h"
#include "Profiler.h"

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

		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11: 
		{
			*outConstantBuffer = new DirectX11ConstantBuffer(buffer, stride);
			return true;
		}
		}
		DebugAssert(false, "Invalid Rendering API for Constant buffer.");
		return false;
	}

	bool ConstantBuffer::Create(std::shared_ptr<ConstantBuffer>& outConstantBuffer, const void* buffer, std::size_t stride)
	{
		PROFILE_SCOPE(CreateConstantBuffer, Rendering);

		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			outConstantBuffer = std::make_shared<DirectX11ConstantBuffer>(buffer, stride);
			return true;
		}
		}
		DebugAssert(false, "Invalid Rendering API for Constant buffer.");
		return false;
	}
}