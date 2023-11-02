#include "EnginePCH.h"
#include "ComputeShader.h"

#include "RenderingAPI.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11ComputeShader.h"
#endif

namespace Engine
{

	bool ComputeShader::Create(ComputeShader** shader)
	{
		// TODO: Metal Integration.

#if defined(GRAPHICS_API_DIRECTX11)
		*shader = new DirectX11ComputeShader();
		return true;
#else
        JKORN_ENGINE_ASSERT(false, "Invalid Rendering API type.");
		return false;
#endif
	}

	bool ComputeShader::Create(std::shared_ptr<ComputeShader>& shader)
	{
		// TODO: Metal Integration.
#if defined(GRAPHICS_API_DIRECTX11)
		shader = std::make_shared<DirectX11ComputeShader>();
		return true;
#else
        JKORN_ENGINE_ASSERT(false, "Invalid Rendering API type.");
		return false;
#endif
	}
}
