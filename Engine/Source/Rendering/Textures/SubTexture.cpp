#include "EnginePCH.h"
#include "SubTexture.h"

#include "Texture.h"

namespace Engine
{
	SubTexture::SubTexture(Texture* texture)
		: m_texture(texture), 
		m_defaultUVs(true), 
		m_size((float)texture->GetWidth(), (float)texture->GetHeight())
	{
		m_uvs[0] = Vec2(0.0f, 1.0f);
		m_uvs[1] = Vec2(1.0f, 1.0f);
		m_uvs[2] = Vec2(1.0f, 0.0f);
		m_uvs[3] = Vec2(0.0f, 0.0f);
	}

	SubTexture::SubTexture(Texture* texture, const Vec2& minUV, const Vec2& maxUV)
		: m_texture(texture), m_defaultUVs(false), m_size()
	{
		m_uvs[0] = Vec2(minUV.x, maxUV.y);
		m_uvs[1] = Vec2(maxUV.x, maxUV.y);
		m_uvs[2] = Vec2(maxUV.x, minUV.y);
		m_uvs[3] = Vec2(minUV.x, minUV.y);

		m_size = Vec2((float)texture->GetWidth(), (float)texture->GetHeight()) * (GetMaxUV() - GetMinUV());
	}

	SubTexture::~SubTexture() { }

	bool SubTexture::HasDefaultUVS() const { return m_defaultUVs; }

	const Vec2* SubTexture::GetUVS() const
	{
		return m_uvs;
	}

	const Vec2& SubTexture::GetMinUV() const
	{
		return m_uvs[3];
	}

	const Vec2& SubTexture::GetMaxUV() const
	{
		return m_uvs[1];
	}

	const Vec2& SubTexture::GetSize() const
	{
		return m_size;
	}

	Texture* SubTexture::GetTexture() const
	{
		return m_texture;
	}
	
	SubTexture* SubTexture::CreateFromTexCoords(Texture* texture, const Vec2& minTextureCoord, const Vec2& maxTextureCoord)
	{
		Vec2 min = Vec2(minTextureCoord.x / (float)texture->GetWidth(),
			minTextureCoord.y / (float)texture->GetHeight());
		Vec2 max = Vec2(maxTextureCoord.x / (float)texture->GetWidth(),
			maxTextureCoord.y / (float)texture->GetHeight());
		return new SubTexture(texture, min, max);
	}
	
	SubTexture* SubTexture::CreateFromSizeAndTexCoord(Texture* texture, const Vec2& texturePos, const Vec2& size)
	{
		return CreateFromTexCoords(texture, texturePos, texturePos + size);
	}
	
	SubTexture* SubTexture::CreateFromTextureAtlas(Texture* texture, const Vec2& cellSize, const Vec2Int& atlasRowCol, const Vec2& scale)
	{
		Vec2 minTex = Vec2((cellSize.x * (float)atlasRowCol.x) / (float)texture->GetWidth(),
			(cellSize.y * (float)atlasRowCol.y) / (float)texture->GetHeight());
		Vec2 sizeTex = Vec2((cellSize.x * scale.x) / (float)texture->GetWidth(),
			(cellSize.y * scale.y) / (float)texture->GetHeight());
		return new SubTexture(texture, minTex, minTex + sizeTex);
	}
}