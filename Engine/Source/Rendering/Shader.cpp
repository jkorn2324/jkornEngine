#include "EnginePCH.h"
#include "Shader.h"

#include "RenderingAPI.h"
#include "DirectX11Shader.h"
#include "Rendering\BufferLayout.h"

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
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			*asset = new DirectX11Shader();
			return true;
		}
		}
		DebugAssert(false, "Unsupported shader type.");
		return false;
	}

	bool Shader::Create(std::shared_ptr<Shader>& shader)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			shader = std::make_shared<DirectX11Shader>();
			return true;
		}
		}
		DebugAssert(false, "Unsupported shader type.");
		return false;
	}

	bool Shader::Create(Shader** shader, const BufferLayout& bufferLayout)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			*shader = new DirectX11Shader(bufferLayout);
			return true;
		}
		}
		DebugAssert(false, "Unsupported shader type.");
		return false;
	}

	bool Shader::Create(std::shared_ptr<Shader>& shader, const BufferLayout& bufferLayout)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			shader = std::make_shared<DirectX11Shader>(bufferLayout);
			return true;
		}
		}
		DebugAssert(false, "Unsupported shader type.");
		return false;
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