#include "EnginePCH.h"
#include "ComputeShader.h"

#include "DirectX11ComputeShader.h"
#include "RenderingAPI.h"

namespace Engine
{

	bool ComputeShader::Create(ComputeShader** shader)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				*shader = new DirectX11ComputeShader();
				return true;
			}
		}
		DebugAssert(false, "Invalid Rendering API type.");
		return false;
	}

	bool ComputeShader::Create(std::shared_ptr<ComputeShader>& shader)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				shader = std::make_shared<DirectX11ComputeShader>();
				return true;
			}
		}
		DebugAssert(false, "Invalid Rendering API type.");
		return false;
	}
}