#include "EnginePCH.h"
#include "Material.h"

#include "JsonFileReader.h"
#include "JsonUtils.h"

#include <rapidjson/stringbuffer.h>

namespace Engine
{
	static ConstantBuffer* s_internalMaterialConstantBuffer = nullptr;
	static uint32_t s_numMaterials = 0;

	Material::Material(const MaterialConstantsLayout& layout)
		: Material(MaterialConstants(layout))
	{
	}

	Material::Material(const MaterialConstants& materialConstants)
		: m_shader(),
		m_materialConstantBuffer(nullptr),
		m_materialConstants(materialConstants),
		m_internalMaterialConstants({}),
		m_buffer(materialConstants.GetLayoutBufferSize()),
		m_numTextures(1)
	{
		if (s_numMaterials <= 0 && !s_internalMaterialConstantBuffer)
		{
			ConstantBuffer::Create(
				&s_internalMaterialConstantBuffer,
				&m_internalMaterialConstants, sizeof(InternalMaterialConstants));
		}
		s_numMaterials++;
		RefreshBuffer();
	}

	Material::Material(const Material& material)
		: m_shader(material.m_shader),
		m_materialConstantBuffer(nullptr),
		m_materialConstants(material.m_materialConstants),
		m_internalMaterialConstants(material.m_internalMaterialConstants),
		m_buffer(material.m_materialConstants.GetLayoutBufferSize()),
		m_numTextures(material.m_numTextures)
	{
		// Copy over the textures.
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData(
				material.m_textures[i].texture);
		}
		if (s_numMaterials <= 0 && !s_internalMaterialConstantBuffer)
		{
			ConstantBuffer::Create(
				&s_internalMaterialConstantBuffer,
				&m_internalMaterialConstants, sizeof(InternalMaterialConstants));
		}
		s_numMaterials++;
		RefreshBuffer();
	}

	Material::~Material()
	{
		s_numMaterials--;
		if (s_numMaterials <= 0)
		{
			delete s_internalMaterialConstantBuffer;
		}
		delete m_materialConstantBuffer;
	}

	Material& Material::operator=(const Material& material)
	{
		delete m_materialConstantBuffer;

		m_shader = material.m_shader;
		m_numTextures = material.m_numTextures;
		m_materialConstants = material.m_materialConstants;
		m_internalMaterialConstants = material.m_internalMaterialConstants;
		m_buffer = Buffer(m_materialConstants.GetLayoutBufferSize());

		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData(
				material.m_textures[i].texture);
		}
		RefreshBuffer();
		return *this;
	}

	void Material::RefreshBuffer()
	{
		if (m_materialConstantBuffer != nullptr)
		{
			m_materialConstantBuffer->SetData(
				m_buffer.GetBuffer(), m_materialConstants.GetLayoutBufferSize());
		}
		else
		{
			ConstantBuffer::Create(
				&m_materialConstantBuffer,
				m_buffer.GetBuffer(),
				m_buffer.GetBufferSize());
		}
	}

	void Material::SetShader(Shader* shader)
	{
		m_shader = shader;
	}

	void Material::SetTextureData(uint32_t slot, const MaterialTextureData& materialTextureData)
	{
		SetTexture(slot, materialTextureData.texture);
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
			m_buffer.GetBuffer(), m_buffer.GetBufferSize());
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
}
