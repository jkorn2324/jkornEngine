#pragma once

#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"

#include "ConstantBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include <stdint.h>

#include "MaterialConstants.h"

namespace Engine
{
	const uint32_t MATERIAL_CONSTANT_BUFFER_SLOT = 2;

	struct MaterialTextureData
	{
		Texture* texture = nullptr;

		MaterialTextureData()
			: texture(nullptr) { }
		MaterialTextureData(Texture* texture)
			: texture(texture) { }
	};

	class Material
	{
	public:
		explicit Material(const MaterialConstantsLayout& layout);
		~Material();

		void SetShader(class Shader* shader);
		void SetTexture(uint32_t slot, Texture* texture);

		const MaterialTextureData& GetTextureData(uint32_t slot) const { return m_textures[slot]; }
		const MaterialConstants& GetMaterialConstants() const { return m_materialConstants; }
		MaterialConstants& GetMaterialConstants() { return m_materialConstants; }

		void Bind() const;
		void Bind(uint32_t constantBufferSlot) const;

		friend bool operator==(const Material& a, const Material& b)
		{
			return a.m_shader == b.m_shader;
		}

		friend bool operator!=(const Material& a, const Material& b)
		{
			return a.m_shader != b.m_shader;
		}

	private:
		Shader* m_shader;
		ConstantBuffer* m_materialConstantBuffer;
		MaterialTextureData* m_textures;
		MaterialConstants m_materialConstants;
		uint32_t m_numTextures;
	};
}