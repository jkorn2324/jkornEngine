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

	bool Shader::Create(Shader** asset)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *asset = new DirectX11Shader();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported shader type.");
		return false;
#endif
	}

	bool Shader::Create(std::shared_ptr<Shader>& shader)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        shader = std::make_shared<DirectX11Shader>();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported shader type.");
        return false;
#endif
	}

	bool Shader::Create(Shader** shader, const BufferLayout& bufferLayout)
	{
#if defined(GRAPHICS_API_DIRECTX11)
		*shader = new DirectX11Shader(bufferLayout);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported shader type.");
        return false;
#endif
	}

	bool Shader::Create(std::shared_ptr<Shader>& shader, const BufferLayout& bufferLayout)
	{
#if defined(GRAPHICS_API_DIRECTX11)
			shader = std::make_shared<DirectX11Shader>(bufferLayout);
			return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported shader type.");
        return false;
#endif
	}

	bool Shader::LoadFromFile(Shader** shader, const wchar_t* fileName, const BufferLayout& bufferLayout)
	{
		if (!Shader::Create(shader, bufferLayout)) return false;
		
		Shader* shaderRef = *shader;
		if (!shaderRef->LoadFromFile_Internal(fileName))
		{
			delete shaderRef;
			*shader = nullptr;
			return false;
		}
		return true;
	}

	bool Shader::LoadFromFile(std::shared_ptr<Shader>& shader, const wchar_t* fileName, const BufferLayout& bufferLayout)
	{
		if (!Shader::Create(shader, bufferLayout)) return false;
		if (!shader->LoadFromFile_Internal(fileName))
		{
			shader.reset();
			return false;
		}
		return true;
	}
}
