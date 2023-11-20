#include "EnginePCH.h"
#include "SubTexture.h"

#include "Texture.h"

namespace Engine
{
	const size_t c_MinUVIndex = 3;
	const size_t c_MaxUVIndex = 1;

	SubTextureContext::SubTextureContext(float width, float height)
		: m_size(width, height)
	{
		m_uvs[0] = Vec2(0.0f, 1.0f);
		m_uvs[1] = Vec2(1.0f, 1.0f);
		m_uvs[2] = Vec2(1.0f, 0.0f);
		m_uvs[3] = Vec2(0.0f, 0.0f);
	}

	SubTextureContext::SubTextureContext(float width, float height, const Vec2& minUV, const Vec2& maxUV)
	{
		m_uvs[0] = Vec2(minUV.x, maxUV.y);
		m_uvs[1] = Vec2(maxUV.x, maxUV.y);
		m_uvs[2] = Vec2(maxUV.x, minUV.y);
		m_uvs[3] = Vec2(minUV.x, minUV.y);

		m_size = Vec2(width, height) * (GetMaxUV() - GetMinUV());
	}

	bool SubTextureContext::HasDefaultUVS() const 
	{ 
		return m_uvs[0] == Vec2::UnitY
			&& m_uvs[1] == Vec2::One
			&& m_uvs[2] == Vec2::UnitX
			&& m_uvs[3] == Vec2::Zero; 
	}

	const Vec2* SubTextureContext::GetUVS() const
	{
		return m_uvs;
	}

	const Vec2& SubTextureContext::GetMinUV() const
	{
		return m_uvs[c_MinUVIndex];
	}

	const Vec2& SubTextureContext::GetMaxUV() const
	{
		return m_uvs[c_MaxUVIndex];
	}

	const Vec2& SubTextureContext::GetSize() const
	{
		return m_size;
	}

	SubTextureContext SubTextureContext::CreateFromTexCoords(Texture* texture, const Vec2& minTextureCoord, const Vec2& maxTextureCoord)
	{
		JKORN_ENGINE_ASSERT(texture != nullptr && texture->IsValid(), "The texture is invalid.");
		float width = (float)texture->GetWidth();
		float height = (float)texture->GetHeight();
		Vec2 min = Vec2(minTextureCoord.x / width,
			minTextureCoord.y / height);
		Vec2 max = Vec2(maxTextureCoord.x / width,
			maxTextureCoord.y / height);
		return SubTextureContext(width, height, min, max);
	}
	
	SubTextureContext SubTextureContext::CreateFromSizeAndTexCoord(Texture* texture, const Vec2& texturePos, const Vec2& size)
	{
		return CreateFromTexCoords(texture, texturePos, texturePos + size);
	}
	
	SubTextureContext SubTextureContext::CreateFromTextureAtlas(Texture* texture, const Vec2& cellSize, const Vec2Int& atlasRowCol, const Vec2& scale)
	{
		JKORN_ENGINE_ASSERT(texture != nullptr && texture->IsValid(), "The texture is invalid.");
		float width = (float)texture->GetWidth();
		float height = (float)texture->GetHeight();
		Vec2 minTex = Vec2((cellSize.x * (float)atlasRowCol.x) / width,
			(cellSize.y * (float)atlasRowCol.y) / height);
		Vec2 sizeTex = Vec2((cellSize.x * scale.x) / width,
			(cellSize.y * scale.y) / height);
		return SubTextureContext(width, height, minTex, minTex + sizeTex);
	}
}