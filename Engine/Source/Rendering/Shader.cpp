#include "EnginePCH.h"
#include "Shader.h"

#include "RenderingAPI.h"
#include "DirectX11Shader.h"
#include "Rendering\BufferLayout.h"

#include "AssetSerializer.h"

namespace Engine
{
	Shader::Shader(const BufferLayout& bufferLayout)
		: m_bufferLayout(bufferLayout)
	{
	}

	bool Shader::SerializeToMetaFile(Shader& shader, AssetSerializationMetaFileData& metaData)
	{
		// Writes the guid to the meta file.
		JsonFileWriter fileWriter(metaData.metaFilePath);
		fileWriter.Write("GUID", metaData.guid);

		// Writes the buffer layout
		fileWriter.BeginObject("BufferLayout");
		fileWriter.Write("NumElements", shader.m_bufferLayout.GetNumElements());
		
		// Writes the buffer layout elements.
		{
			fileWriter.BeginArray("Elements");
			for (uint32_t i = 0; i < shader.m_bufferLayout.GetNumElements(); i++)
			{
				fileWriter.BeginObject();
				BufferLayoutParam& param = shader.m_bufferLayout.parameters[i];
				fileWriter.Write("Name", (std::string)param.name);
				fileWriter.Write("LayoutType", (uint32_t)param.layoutType);
				fileWriter.Write("Offset", param.offset);
				fileWriter.EndObject();
			}
			fileWriter.EndArray();
		}

		fileWriter.EndObject();	
		fileWriter.Flush();
		return true;
	}

	bool Shader::DeserializeMetaFile(Shader& shader, AssetDeserializationMetaFileData& metaData)
	{
		JsonFileParser fileParser(metaData.metaFilePath);
		if (!fileParser.IsValid()) return false;
		
		rapidjson::Document& document = fileParser.GetDocument();
		if (document.HasMember("BufferLayout"))
		{
			rapidjson::Value& bufferLayout = document["BufferLayout"].GetObject();
			uint32_t numElements;
			ReadUint32(bufferLayout, "NumElements", numElements);

			std::string elementName;
			BufferLayoutType bufferLayoutType;
			uint32_t offset;

			rapidjson::Value& elements = document["Elements"].GetArray();
			for (uint32_t i = 0; i < numElements; ++i)
			{
				rapidjson::Value& elementObject = elements[i].GetObject();
				ReadString(elementObject, "Name", elementName);
				ReadEnum<BufferLayoutType>(elementObject, "LayoutType", bufferLayoutType);
				ReadUint32(elementObject, "Offset", offset);
				shader.m_bufferLayout.parameters.push_back(
					BufferLayoutParam(elementName, offset, bufferLayoutType));
			}
		}
		return true;
	}

	bool Shader::DeserializeFromFile(Shader& shader, AssetDeserializationFileData& value)
	{
		if (shader.m_bufferLayout.GetNumElements() > 0)
		{
			return DeserializeFromFile(shader, value, shader.m_bufferLayout);
		}
		return false;
	}

	bool Shader::DeserializeFromFile(Shader& shader, AssetDeserializationFileData& value, const BufferLayout& bufferLayout)
	{
		return shader.Load(value.filePath.c_str(), bufferLayout);
	}

	// Doesn't do anything as the shaders themselves aren't serialized.
	bool Shader::SerializeToFile(Shader& shader, AssetSerializationFileData& metaData)
	{
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