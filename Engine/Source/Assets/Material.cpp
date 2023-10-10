#include "EnginePCH.h"
#include "Material.h"

#include "JsonFileParser.h"
#include "JsonUtils.h"

#include "AssetSerializer.h"
#include "AssetManager.h"
#include "AssetCache.h"
#include "AssetMapper.h"

#include <rapidjson/stringbuffer.h>

namespace Engine
{
	static ConstantBuffer* s_internalMaterialConstantBuffer = nullptr;
	static uint32_t s_numMaterials = 0;

	Material::Material()
		: m_shader(),
		m_materialConstantBuffer(nullptr),
		m_textures(nullptr),
		m_numTextures(1),
		m_materialConstants({}),
		m_internalMaterialConstants({})
	{
		if (s_numMaterials <= 0)
		{
			s_internalMaterialConstantBuffer = ConstantBuffer::Create(
				&m_internalMaterialConstants, sizeof(InternalMaterialConstants));
		}
		s_numMaterials++;

		m_textures = new MaterialTextureData[m_numTextures];
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData();
		}
	}

	Material::Material(const MaterialConstantsLayout& layout)
		: m_shader(),
		m_materialConstantBuffer(nullptr),
		m_textures(nullptr),
		m_numTextures(1),
		m_materialConstants(layout),
		m_internalMaterialConstants({})
	{
		if (s_numMaterials <= 0)
		{
			s_internalMaterialConstantBuffer = ConstantBuffer::Create(
				&m_internalMaterialConstants, sizeof(InternalMaterialConstants));
		}
		s_numMaterials++;

		m_textures = new MaterialTextureData[m_numTextures];
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData();
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			m_materialConstants.GetRawBuffer(),
			m_materialConstants.GetBufferSize());
	}

	Material::Material(const Material& material)
		: m_shader(material.m_shader),
		m_materialConstantBuffer(nullptr),
		m_textures(nullptr),
		m_numTextures(material.m_numTextures),
		m_materialConstants(material.m_materialConstants),
		m_internalMaterialConstants(material.m_internalMaterialConstants)
	{
		s_numMaterials++;
		m_textures = new MaterialTextureData[m_numTextures];
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData(
				material.m_textures[i].texture);
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			m_materialConstants.GetRawBuffer(),
			m_materialConstants.GetBufferSize());
	}

	Material::~Material()
	{
		s_numMaterials--;

		if (s_numMaterials <= 0)
		{
			delete s_internalMaterialConstantBuffer;
		}

		delete[] m_textures;
		delete m_materialConstantBuffer;
	}

	Material& Material::operator=(const Material& material)
	{
		delete[] m_textures;
		delete m_materialConstantBuffer;

		m_shader = material.m_shader;
		m_numTextures = material.m_numTextures;
		m_materialConstants = material.m_materialConstants;
		m_internalMaterialConstants = material.m_internalMaterialConstants;

		m_textures = new MaterialTextureData[m_numTextures];
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData(
				material.m_textures[i].texture);
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			m_materialConstants.GetRawBuffer(),
			m_materialConstants.GetBufferSize());
		return *this;
	}

	void Material::SetConstantsLayout(const MaterialConstantsLayout& layout)
	{
		m_materialConstants = MaterialConstants(layout);
		
		if (m_materialConstantBuffer != nullptr)
		{
			delete m_materialConstantBuffer;
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			m_materialConstants.GetRawBuffer(),
			m_materialConstants.GetBufferSize());
	}

	void Material::SetConstantsLayout(const MaterialConstantsLayout& layout, size_t layoutSize)
	{
		m_materialConstants = MaterialConstants(layout, layoutSize);
		if (m_materialConstantBuffer != nullptr)
		{
			delete m_materialConstantBuffer;
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			m_materialConstants.GetRawBuffer(),
			m_materialConstants.GetBufferSize());
	}

	void Material::SetShader(const AssetRef<Shader>& shader)
	{
		m_shader = shader;
	}

	void Material::SetTextureData(uint32_t slot, const MaterialTextureData& materialTextureData)
	{
		SetTexture(slot, materialTextureData.texture);
	}

	void Material::SetTexture(uint32_t slot, const AssetRef<Texture>& texture)
	{
		if (slot >= m_numTextures)
		{
			return;
		}

		switch (slot)
		{
		// Default Texture Slot.
		case 0:
		{
			if (!texture)
			{
				// Removes the flag only if it exists.
				if (m_internalMaterialConstants.c_materialFlags & MaterialFlag_DefaultTexture)
				{
					m_internalMaterialConstants.c_materialFlags ^= MaterialFlag_DefaultTexture;
				}
			}
			else
			{
				m_internalMaterialConstants.c_materialFlags |= MaterialFlag_DefaultTexture;
			}
			break;
		}
		}
		MaterialTextureData& materialTextureData = m_textures[slot];
		materialTextureData.texture = texture;
	}

	void Material::Bind() const
	{
		Bind(PER_SHADER_CONSTANTS_SLOT);
	}

	void Material::Bind(uint32_t constantBufferSlot) const
	{
		if (m_materialConstantBuffer == nullptr
			|| !m_shader)
		{
			return;
		}

		m_shader->Bind();
		
		m_materialConstantBuffer->SetData(
			m_materialConstants.GetRawBuffer(), m_materialConstants.GetBufferSize());
		m_materialConstantBuffer->Bind(constantBufferSlot,
			PIXEL_SHADER | VERTEX_SHADER);

		s_internalMaterialConstantBuffer->SetData(
			&m_internalMaterialConstants, sizeof(m_internalMaterialConstants));
		s_internalMaterialConstantBuffer->Bind(MATERIAL_CONSTANTS_SLOT,
			PIXEL_SHADER | VERTEX_SHADER);

		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			const auto& texture = m_textures[i];
			if (texture.texture)
			{
				texture.texture->Bind(i);
			}
		}
	}

#pragma region serialization

	static MaterialConstantLayoutAttribute LoadMaterialAttributeConstantLayoutType(rapidjson::Value& value,
		char*& valueBuffer, size_t& offset)
	{
		std::string name;
		MaterialConstantLayoutType layoutType;
		bool padding = false;

		ReadString(value, "Name", name);
		ReadEnum<MaterialConstantLayoutType>(value, "Type", layoutType);
		ReadBool(value, "Pad", padding);

		MaterialConstantLayoutAttribute attribute = { name, layoutType, padding };

		// Appends the attribute data to the value buffer.
		char* ptrValue = valueBuffer + offset;
		switch (layoutType)
		{
		case LayoutType_Bool:
		{
			bool boolValue;
			ReadBool(value, "Value", boolValue);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&boolValue), attribute.layoutStride);
			break;
		}
		case LayoutType_Float:
		{
			float floatValue;
			ReadFloat(value, "Value", floatValue);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&floatValue), attribute.layoutStride);
			break;
		}
		case LayoutType_Int16:
		{
			int16_t int16Value;
			ReadInt16(value, "Value", int16Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&int16Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Int32:
		{
			int32_t int32Value;
			ReadInt32(value, "Value", int32Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&int32Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Int64:
		{
			int64_t int64Value;
			ReadInt64(value, "Value", int64Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&int64Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Uint16:
		{
			uint16_t int16Value;
			ReadUint16(value, "Value", int16Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&int16Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Uint32:
		{
			uint32_t int32Value;
			ReadUint32(value, "Value", int32Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&int32Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Uint64:
		{
			uint64_t int64Value;
			ReadUint64(value, "Value", int64Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&int64Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Vector2:
		{
			MathLib::Vector2 vector2Value;
			ReadVector2(value, "Value", vector2Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&vector2Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Vector3:
		{
			MathLib::Vector3 vector3Value;
			ReadVector3(value, "Value", vector3Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&vector3Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Vector4:
		{
			MathLib::Vector4 vector4Value;
			ReadVector4(value, "Value", vector4Value);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&vector4Value), attribute.layoutStride);
			break;
		}
		case LayoutType_Quaternion:
		{
			MathLib::Quaternion quaternionValue;
			ReadQuaternion(value, "Value", quaternionValue);
			std::memcpy(ptrValue, reinterpret_cast<char*>(&quaternionValue), attribute.layoutStride);
			break;
		}
		}
		offset += attribute.layoutStride;
		return attribute;
	}

	bool Material::DeserializeFromFile(Material& material, AssetDeserializationFileData& value)
	{
		JsonFileParser jsonFileParser(value.filePath);
		if (!jsonFileParser.IsValid())
		{
			return false;
		}
		
		MaterialConstantsLayout layout;
		char* materialConstantBuffer = nullptr;
		rapidjson::Document& document = jsonFileParser.GetDocument();
		
		size_t layoutByteSize = 0;
		// Deserializes the buffer layout with values.
		if (document.HasMember("Layout"))
		{
			rapidjson::Value& layoutValue = document["Layout"].GetObject();
			if (!ReadSize(layoutValue, "Size", layoutByteSize)
				|| layoutByteSize <= 0)
			{
				return false;
			}
			materialConstantBuffer = new char[layoutByteSize];
			size_t currentOffset = 0;
			if (layoutValue.HasMember("Attributes"))
			{
				rapidjson::Value& attributes = layoutValue["Attributes"].GetArray();
				for (rapidjson::SizeType i = 0; i < attributes.Size(); i++)
				{
					rapidjson::Value& attributeValue = attributes[i].GetObject();
					MaterialConstantLayoutAttribute attribute
						= LoadMaterialAttributeConstantLayoutType(attributeValue, materialConstantBuffer, currentOffset);
					layout.layoutAttributes.push_back(attribute);
				}
			}
		}

		// Applies the material constants.
		{
			material.SetConstantsLayout(layout, layoutByteSize);
			MaterialConstants& constants = material.GetMaterialConstants();
			constants.SetRawBuffer(materialConstantBuffer);
			delete[] materialConstantBuffer;
		}

		// Reads & Loads the shader from its GUID.
		{
			uint64_t shaderGUID;
			ReadUint64(document, "Shader", shaderGUID);

			std::filesystem::path assetPath;
			if (AssetManager::GetAssetMapper().GetPath(shaderGUID, assetPath))
			{
				AssetManager::GetShaders().Load(material.m_shader, assetPath);
			}
		}

		// Reads and loads the textures from its GUIDs.
		{
			uint64_t currentTextureGUID;
			if (document.HasMember("Textures"))
			{
				ReadUint32(document, "NumTextures", material.m_numTextures);
				rapidjson::Value& texturesArray = document["Textures"].GetArray();
				for (uint32_t i = 0; i < material.m_numTextures; i++)
				{
					rapidjson::Value& textureValue = texturesArray[i].GetObject();
					ReadUint64(textureValue, "GUID", currentTextureGUID);

					std::filesystem::path path;
					if (currentTextureGUID != 0
						&& AssetManager::GetAssetMapper().GetPath(currentTextureGUID, path))
					{
						GUID guid(currentTextureGUID);
						AssetRef<Texture> texture;
						AssetManager::GetTextures().Load(texture, path);
						material.SetTexture(i, texture);
					}
				}
			}
		}
		return true;
	}

	bool Material::SerializeToFile(Material& material, AssetSerializationFileData& metaData)
	{
		// Writes to a material file.
		JsonFileWriter fileWriter(metaData.filePath);
		
		// Writes the constant buffer attribute layout.
		{
			fileWriter.BeginObject("Layout");
			fileWriter.Write("Size", material.m_materialConstants.m_totalBufferSize);
			fileWriter.BeginArray("Attributes");

			for (auto& pair : material.m_materialConstants.m_materialConstants)
			{
				fileWriter.BeginObject();
				fileWriter.Write("Name", pair.first);
				fileWriter.Write<uint32_t>("Type", pair.second.layoutType);
				fileWriter.Write<bool>("Pad", pair.second.pad);
				
				// Writes the Value of the given type.
				switch (pair.second.layoutType)
				{
				case LayoutType_Bool:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<bool>(pair.first));
					break;
				}
				case LayoutType_Float:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<float>(pair.first));
					break;
				}
				case LayoutType_Int16:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<int16_t>(pair.first));
					break;
				}
				case LayoutType_Int32:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<int32_t>(pair.first));
					break;
				}
				case LayoutType_Int64:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<int64_t>(pair.first));
					break;
				}
				case LayoutType_Uint16:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<uint16_t>(pair.first));
					break;
				}
				case LayoutType_Uint32:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<uint32_t>(pair.first));
					break;
				}
				case LayoutType_Uint64:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<uint64_t>(pair.first));
					break;
				}
				case LayoutType_Quaternion:
				{
					fileWriter.Write("Value", 
						*material.m_materialConstants.GetMaterialConstant<MathLib::Quaternion>(pair.first));
					break;
				}
				case LayoutType_Vector2:
				{
					fileWriter.Write("Value",
						*material.m_materialConstants.GetMaterialConstant<MathLib::Vector2>(pair.first));
					break;
				}
				case LayoutType_Vector3:
				{
					fileWriter.Write("Value",
						*material.m_materialConstants.GetMaterialConstant<MathLib::Vector3>(pair.first));
					break;
				}
				case LayoutType_Vector4:
				{
					fileWriter.Write("Value",
						*material.m_materialConstants.GetMaterialConstant<MathLib::Vector4>(pair.first));
					break;
				}
				}
				fileWriter.EndObject();
			}

			fileWriter.EndArray();
			fileWriter.EndObject();
		}

		// Writes the shadedr to the json.
		if (material.HasShader())
		{
			GUID shaderGUID;
			material.m_shader.GetGUID(shaderGUID);
			fileWriter.Write("Shader", (uint64_t)shaderGUID);
		}
		else
		{
			fileWriter.Write("Shader", 0);
		}

		// Writes the textures to a json.
		{
			fileWriter.Write("NumTextures", material.m_numTextures);
			fileWriter.BeginArray("Textures");
			GUID guid;

			for (uint32_t i = 0; i < material.m_numTextures; i++)
			{
				MaterialTextureData materialTextureData = material.m_textures[i];
				fileWriter.BeginObject();
				if (materialTextureData.texture)
				{
					materialTextureData.texture.GetGUID(guid);
					fileWriter.Write("GUID", (uint64_t)guid);
				}
				else
				{
					fileWriter.Write("GUID", 0);
				}
				fileWriter.EndObject();
			}
			fileWriter.EndArray();
		}
		fileWriter.Flush();
		return true;
	}

	bool Material::SerializeToMetaFile(Material& material, AssetSerializationMetaFileData& metaData)
	{
		{
			JsonFileWriter writer(metaData.metaFilePath);
			writer.Write("GUID", (uint64_t)metaData.guid);
			writer.Flush();
		}
		return true;
	}

	bool Material::DeserializeMetaFile(Material& material, AssetDeserializationMetaFileData& metaData)
	{
		return true;
	}

#pragma endregion

	bool Material::Create(Material** material)
	{
		*material = new Material();
		return true;
	}

	bool Material::Create(Material** material, const MaterialConstantsLayout& constants)
	{
		*material = new Material(constants);
		return true;
	}

	bool Material::Create(std::shared_ptr<Material>& material)
	{
		material = std::make_shared<Material>();
		return true;
	}

	bool Material::Create(std::shared_ptr<Material>& material, const MaterialConstantsLayout& constants)
	{
		material = std::make_shared<Material>(constants);
		return true;
	}
}
