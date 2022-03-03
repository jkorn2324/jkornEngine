#include "EnginePCH.h"
#include "Material.h"

#include "JsonFileParser.h"
#include "JsonUtils.h"

#include <rapidjson\stringbuffer.h>

namespace Engine
{
	static ConstantBuffer* s_internalMaterialConstantBuffer = nullptr;
	static uint32_t s_numMaterials = 0;

	Material::Material()
		: m_shader(nullptr),
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
			m_textures[i] = MaterialTextureData(nullptr);
		}
	}

	Material::Material(const MaterialConstantsLayout& layout)
		: m_shader(nullptr),
		m_materialConstantBuffer(nullptr),
		m_textures(nullptr),
		m_numTextures(1),
		m_materialConstants(layout),
		m_internalMaterialConstants({}),
		m_assetGUID()
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
			m_textures[i] = MaterialTextureData(nullptr);
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
		m_internalMaterialConstants(material.m_internalMaterialConstants),
		m_assetGUID()
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

	void Material::SetShader(Shader* shader)
	{
		m_shader = shader;
	}

	void Material::SetTexture(uint32_t slot, Texture* texture)
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
			if (texture == nullptr)
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
			|| m_shader == nullptr)
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
			if (texture.texture != nullptr)
			{
				texture.texture->Bind(i);
			}
		}
	}

	void Material::SerializeToMetaFile(Material& material, AssetSerializationMetaData& prettyWriter)
	{
		// TODO: Read from meta file.
	}


	bool Material::DeserializeFromMetaFile(Material& material, AssetDeserializationMetaData& value)
	{
		// TODO: Write to a meta file.
		return false;
	}


	bool Material::DeserializeFromFile(Material& material, AssetDeserializationFileData& value)
	{
		// TODO: Write to a meta file.
		return false;
	}

	Material* Material::Create(const MaterialConstantsLayout& constants)
	{
		return new Material(constants);
	}

	Material* Material::Create()
	{
		return new Material;
	}
}