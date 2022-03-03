#include "EnginePCH.h"
#include "Shader.h"

#include "RenderingAPI.h"
#include "DirectX11Shader.h"
#include "Rendering\BufferLayout.h"

#include "AssetSerializer.h"

namespace Engine
{

	void Shader::SerializeToMetaFile(Shader& shader, AssetSerializationMetaData& prettyWriter)
	{
		// TODO: Write to meta file.
	}


	bool Shader::DeserializeFromMetaFile(Shader& shader, AssetDeserializationMetaData& value)
	{
		// TODO: Read from a meta file.
		return false;
	}

	bool Shader::DeserializeFromFile(Shader& shader, AssetDeserializationFileData& value)
	{
		return false;
	}

	bool Shader::DeserializeFromFile(Shader& shader, AssetDeserializationFileData& value, const BufferLayout& bufferLayout)
	{
		return shader.Load(value.filePath.c_str(), bufferLayout);
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