#include "EnginePCH.h"
#include "ComputeShader.h"

#include "DirectX11ComputeShader.h"
#include "RenderingAPI.h"
#include "AssetSerializer.h"

namespace Engine
{

	bool ComputeShader::SerializeToMetaFile(ComputeShader& shader, AssetSerializationMetaFileData& metaData)
	{
		// TODO: Implementation
		return true;
	}

	bool ComputeShader::DeserializeMetaFile(ComputeShader& shader, AssetDeserializationMetaFileData& metaData)
	{
		// TODO: Implementation
		return true;
	}

	bool ComputeShader::DeserializeFromFile(ComputeShader& shader, AssetDeserializationFileData& value)
	{
		// TODO: Implementation
		return shader.Load(value.filePath.c_str());
	}

	bool ComputeShader::SerializeToFile(ComputeShader& shader, AssetSerializationFileData& value)
	{
		// TODO: Implementation
		return true;
	}

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