#include "EnginePCH.h"
#include "Shader.h"

#include "RenderingAPI.h"
#include "BufferLayout.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11Shader.h"
#endif

namespace Engine
{
	Shader::Shader(const BufferLayout& bufferLayout)
		: m_bufferLayout(bufferLayout)
	{
	}

	bool Shader::LoadFromFile_Internal(const wchar_t* filePath, const BufferLayout& bufferLayout)
	{
		m_bufferLayout = bufferLayout;
		return LoadFromFile_Internal(filePath);
	}

	Shader* Shader::Create(const BufferLayout& bufferLayout, bool empty)
	{
#if defined(GRAPHICS_API_DIRECTX11)
		return empty ? new DirectX11Shader()
			: new DirectX11Shader(bufferLayout);
#else
		JKORN_ENGINE_ASSERT(false, "Unsupported shader type.");
		return nullptr;
#endif
	}

	Shader* Shader::LoadFromFile(const wchar_t* fileName, const BufferLayout& bufferLayout)
	{
		Shader* createdShader = Create(bufferLayout);
		if (!createdShader)
		{
			return nullptr;
		}
		if (!createdShader->LoadFromFile_Internal(fileName))
		{
			delete createdShader;
			return nullptr;
		}
		return createdShader;
	}
}
