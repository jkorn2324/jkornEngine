#pragma once

#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"

#include <stdint.h>

namespace Engine
{
	const uint32_t MATERIAL_CONSTANT_BUFFER_SLOT = 2;

	class Texture;

	struct MaterialTextureData
	{
		Texture* texture = nullptr;

		MaterialTextureData()
			: texture(nullptr) { }
		MaterialTextureData(Texture* texture)
			: texture(texture) { }
	};

	template<typename Constants>
	class Material
	{
	public:
		explicit Material();
		~Material();

		void SetShader(class Shader* shader);
		void SetTexture(uint32_t slot, Texture* texture);

		void Bind() const;
		void Bind(uint32_t constantBufferSlot) const;

	private:
		class Shader* m_shader;
		ConstantBuffer* m_materialConstantBuffer;
		MaterialTextureData* m_textures;
		uint32_t m_numTextures;

	public:
		Constants materialConstants;
	};

	template<typename Constants>
	Material<Constants>::Material()
		: m_shader(nullptr),
		m_materialConstantBuffer(nullptr),
		m_textures(nullptr),
		m_numTextures(1),
		materialConstants()
	{
		m_textures = new MaterialTextureData[m_numTextures];
		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			m_textures[i] = MaterialTextureData(nullptr);
		}
		m_materialConstantBuffer = ConstantBuffer::Create(
			&materialConstants, sizeof(materialConstants));
	}

	template<typename Constants>
	Material<Constants>::~Material()
	{
		delete[] m_textures;
		delete m_materialConstantBuffer;
	}
	
	template<typename Constants>
	inline void Material<Constants>::SetShader(Shader* shader)
	{
		m_shader = shader;
	}

	template<typename Constants>
	inline void Material<Constants>::SetTexture(uint32_t slot, Texture* texture)
	{
		if (slot >= m_numTextures)
		{
			return;
		}
		MaterialTextureData& materialTextureData = m_textures[slot];
		materialTextureData.texture = texture;
	}
	
	template<typename Constants>
	inline void Material<Constants>::Bind() const
	{
		Bind(MATERIAL_CONSTANT_BUFFER_SLOT);
	}
	
	template<typename Constants>
	inline void Material<Constants>::Bind(uint32_t constantBufferSlot) const
	{
		m_materialConstantBuffer->SetData(
			&materialConstants, sizeof(materialConstants));

		GraphicsRenderer* renderer = GraphicsRenderer::Get();
		renderer->SetShader(m_shader);
		renderer->SetConstantBuffer(constantBufferSlot, m_materialConstantBuffer,
			ConstantBufferFlags::PIXEL_SHADER | ConstantBufferFlags::VERTEX_SHADER);

		for (uint32_t i = 0; i < m_numTextures; i++)
		{
			const auto& texture = m_textures[i];
			if (texture.texture != nullptr)
			{
				renderer->SetTexture(i, texture.texture);
			}
		}
	}
}