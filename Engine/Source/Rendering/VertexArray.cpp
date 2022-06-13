#include "EnginePCH.h"
#include "VertexArray.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"

#include "DirectX11VertexArray.h"

namespace Engine
{
	bool VertexArray::Create(VertexArray** outVertexArray)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				*outVertexArray = new DirectX11VertexArray();
				return true;
			}
		}
		// TODO: Implementation
		DebugAssert(false, "Failed to create Vertex Array.");
		return false;
	}

	bool VertexArray::Create(std::shared_ptr<VertexArray>& outVertexArray)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				outVertexArray = std::make_shared<DirectX11VertexArray>();
				return true;
			}
		}
		DebugAssert(false, "Failed to Create Vertex Array.");
		return false;
	}

	bool VertexArray::Create(std::unique_ptr<VertexArray>& outVertexArray)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				outVertexArray = std::make_unique<DirectX11VertexArray>();
				return true;
			}
		}
		DebugAssert(false, "Failed to Create Vertex Array.");
		return false;
	}
}