#pragma once

#include "Source\Vector.h"
#include "Source\Math.h"
#include "AssetReferenceManager.h"

namespace Engine
{
	using Vec2 = MathLib::Vector2;
	using Vec2Int = MathLib::Vector2Int;

	const uint32_t UV_COUNT = 4;

	class Texture;

	class SubTexture
	{
	public:
		explicit SubTexture(const AssetRef<Texture>& texture);
		explicit SubTexture(const AssetRef<Texture>& texture, const Vec2& minUV,
			const Vec2& maxUV);
		~SubTexture();

		bool HasDefaultUVS() const;
		const Vec2* GetUVS() const;
		const Vec2& GetMinUV() const;
		const Vec2& GetMaxUV() const;

		const Vec2& GetSize() const;

		const AssetRef<Texture>& GetTexture() const;

		static SubTexture* CreateFromTexCoords(const AssetRef<Texture>& texture,
			const Vec2& minTextureCoord, const Vec2& maxTextureCoord);
		static SubTexture* CreateFromSizeAndTexCoord(const AssetRef<Texture>& texture,
			const Vec2& texturePos, const Vec2& size);
		static SubTexture* CreateFromTextureAtlas(const AssetRef<Texture>& texture,
			const Vec2& cellSize, const Vec2Int& atlasRowCol, const Vec2& scale = Vec2::One);

	private:
		AssetRef<Texture> m_texture;
		Vec2 m_uvs[UV_COUNT];
		Vec2 m_size;
		bool m_defaultUVs;
	};
}