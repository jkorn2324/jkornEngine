#include "EnginePCH.h"
#include "Material.h"

namespace Engine
{

	Material::Material(const MaterialConstantsLayout& layout)
		: m_shader(nullptr),
		m_materialConstantBuffer(nullptr),
		m_textures(nullptr),
		m_numTextures(1),
		m_materialConstants(layout)
	{
		m_textures = new MaterialTextureData[m_numTextures];
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData(nullptr);
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			m_materialConstants.GetRawBuffer(),
			m_materialConstants.GetBufferSize());
	}

	Material::~Material()
	{
		delete[] m_textures;
		delete m_materialConstantBuffer;
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
		MaterialTextureData& materialTextureData = m_textures[slot];
		materialTextureData.texture = texture;
	}

	void Material::Bind() const
	{
		Bind(MATERIAL_CONSTANT_BUFFER_SLOT);
	}

	void Material::Bind(uint32_t constantBufferSlot) const
	{
		m_materialConstantBuffer->SetData(
			m_materialConstants.GetRawBuffer(), m_materialConstants.GetBufferSize());

		m_shader->Bind();
		m_materialConstantBuffer->Bind(constantBufferSlot,
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
}