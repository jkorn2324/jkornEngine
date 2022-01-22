#include "EnginePCH.h"
#include "Shader.h"

#include "RenderingAPI.h"
#include "DirectX11Shader.h"
#include "Rendering\BufferLayout.h"

namespace Engine
{

	Shader* Shader::StaticLoad(const std::wstring& fileName, const BufferLayout& bufferLayout)
	{
		Shader* shader = Create();
		DebugAssert(shader != nullptr, "Shader failed to be created.");

		const wchar_t* cstrFileName = fileName.c_str();
		if (!shader->Load(cstrFileName, bufferLayout))
		{
			delete shader;
			return nullptr;
		}
		return shader;
	}

	Shader* Shader::StaticLoad(const std::wstring& fileName, const std::unique_ptr<BufferLayout>& bufferLayout)
	{
		if (bufferLayout.get() == nullptr)
		{
			return nullptr;
		}
		return StaticLoad(fileName, *bufferLayout.get());
	}

	Shader* Shader::Create()
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11Shader();
		case RenderingAPIType::NONE:
		{
			DebugAssert(false, "Unsupported shader type.");
			return nullptr;
		}
		}
		return nullptr;
	}
}