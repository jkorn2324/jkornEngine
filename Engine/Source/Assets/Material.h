#pragma once

#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"

#include "AssetReferenceManager.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "MaterialConstants.h"
#include "EngineMacros.h"
#include "GUID.h"

#include <stdint.h>

namespace Engine
{
	const uint32_t MATERIAL_CONSTANTS_SLOT = 2;
	const uint32_t PER_SHADER_CONSTANTS_SLOT = 3;

	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

	struct MaterialTextureData
	{
		AssetRef<Texture> texture;

		MaterialTextureData()
			: texture() { }
		MaterialTextureData(const AssetRef<Texture>& texture)
			: texture(texture) { }
	};

	enum MaterialFlags
	{
		MaterialFlag_DefaultTexture = 1 << 0
	};

	struct InternalMaterialConstants
	{
		uint32_t c_materialFlags = 0;

	private:
		float pad1, pad2, pad3;
	};

	class Material
	{
	public:
		explicit Material();
		explicit Material(const MaterialConstantsLayout& layout);
		explicit Material(const Material& material);
		~Material();

		Material& operator=(const Material& material);

		void SetConstantsLayout(const MaterialConstantsLayout& layout);
		void SetConstantsLayout(const MaterialConstantsLayout& layout, size_t layoutSize);

		void SetShader(const AssetRef<Shader>& shader);
		void SetTexture(uint32_t slot, const AssetRef<Texture>& texture);

		bool HasShader() const { return m_shader; }

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
		void SetTextureData(uint32_t slot, const MaterialTextureData& materialTextureData);

		AssetRef<Shader> m_shader;
		ConstantBuffer* m_materialConstantBuffer;
		MaterialTextureData* m_textures;
		MaterialConstants m_materialConstants;
		InternalMaterialConstants m_internalMaterialConstants;
		uint32_t m_numTextures;

		static bool Create(Material** material, const MaterialConstantsLayout& constants);
		static bool Create(std::shared_ptr<Material>& material, const MaterialConstantsLayout& constants);

		SERIALIZABLE_ASSET(Material);
	};
}