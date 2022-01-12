#include "EnginePCH.h"
#include "BufferLayout.h"

#include "DirectX11BufferLayout.h"
#include "RenderingAPI.h"

#include <string.h>

namespace Engine
{


	std::unique_ptr<BufferLayout> BufferLayout::Create(const std::initializer_list<BufferLayoutParam>& initializerList)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11: return std::make_unique<DirectX11BufferLayout>(initializerList);
		}
		DebugAssert(false, "Unable to create buffer layout for the platform.");
		return nullptr;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutParam>& layoutParameters) { }
}
