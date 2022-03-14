#include "EnginePCH.h"
#include "Shader.h"

#include "RenderingAPI.h"
#include "DirectX11Shader.h"
#include "Rendering\BufferLayout.h"

#include "AssetSerializer.h"

namespace Engine
{

	bool Shader::DeserializeFromFile(Shader& shader, AssetDeserializationFileData& value)
	{
		// TODO: Load buffer layout from meta file with GUID.
		return false;
	}

	bool Shader::DeserializeFromFile(Shader& shader, AssetDeserializationFileData& value, const BufferLayout& bufferLayout)
	{
		return shader.Load(value.filePath.c_str(), bufferLayout);
	}

	bool Shader::SerializeToFile(Shader& shader, AssetSerializationMetaData& metaData)
	{
		// TODO: Write to a meta file. Should really only contain the GUID.
		return true;
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