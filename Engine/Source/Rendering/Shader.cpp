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
				BufferLayoutParameterSet& params = shader.m_bufferLayout.parameters[i];

				fileWriter.BeginObject();
				fileWriter.Write("NumParams", params.parameters.size());
				fileWriter.BeginArray("ParamsGroup");
				
				for (const BufferLayoutParam& param : params.parameters)
				{
					fileWriter.BeginObject();
					fileWriter.Write("Name", param.name);
					fileWriter.Write("LayoutType", (uint32_t)param.layoutType);
					fileWriter.Write("NumValues", (uint32_t)param.numValues);
					fileWriter.Write("SemanticIndex", (uint32_t)param.semanticIndex);
					fileWriter.Write("SemanticType", (uint32_t)param.semanticType);
					fileWriter.EndObject();
				}
				fileWriter.EndArray();
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
			rapidjson::Value& bufferLayout = document["BufferLayout"].GetArray();
			uint32_t numElements;
			ReadUint32(bufferLayout, "NumElements", numElements);

			rapidjson::Value& elements = document["Elements"].GetArray();
			for (uint32_t i = 0; i < numElements; ++i)
			{
				rapidjson::Value& elementObject = elements[i].GetObject();
				if (elementObject.HasMember("NumParams"))
				{
					uint32_t numParams;
					ReadUint32(elementObject, "NumParams", numParams);
					
					BufferLayoutParameterSet parameters;
					rapidjson::Value& paramsGroup = elementObject["ParamsGroup"].GetArray();
					for (uint32_t i = 0; i < numParams; ++i)
					{
						rapidjson::Value& paramObject = paramsGroup[i].GetObject();
						std::string elementName;
						BufferLayoutType bufferLayoutType;
						BufferLayoutSemanticType semanticType;
						uint32_t semanticIndex;
						uint32_t numValues;

						ReadString(paramObject, "Name", elementName);
						ReadEnum(paramObject, "LayoutType", bufferLayoutType);
						ReadEnum(paramObject, "SemanticType", semanticType);
						ReadUint32(paramObject, "SemanticIndex", semanticIndex);
						ReadUint32(paramObject, "NumValues", numValues);

						parameters.parameters.push_back(
							{ elementName, semanticType, bufferLayoutType, numValues, semanticIndex });
					}

					// Pushes back the parameters to the buffer layout.
					if (parameters.parameters.size() > 0)
					{
						shader.m_bufferLayout.parameters.push_back(parameters);
					}
				}
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

	bool Shader::Create(std::shared_ptr<Shader>& asset)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			asset = std::make_shared<DirectX11Shader>();
			return true;
		}
		}
		DebugAssert(false, "Unsupported shader type.");
		return false;
	}
}