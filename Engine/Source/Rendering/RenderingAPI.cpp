#include "EnginePCH.h"
#include "RenderingAPI.h"

#include "DirectX11RenderingAPI.h"

namespace Engine
{

	RenderingAPIType RenderingAPI::s_renderingAPIType = RenderingAPIType::DIRECTX11;


	RenderingAPI* RenderingAPI::Create()
	{
		switch (s_renderingAPIType)
		{
			case RenderingAPIType::DIRECTX11:	return new DirectX11RenderingAPI();
		}
		DebugAssert(false, "Unsupported rendering api.");
		return nullptr;
	}

}