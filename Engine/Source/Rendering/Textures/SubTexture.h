#pragma once

#include "Source\Vector.h"
#include "Source\Math.h"

namespace Engine
{
	using Vec2 = MathLib::Vector2;
	using Vec2Int = MathLib::Vector2Int;

	const uint32_t UV_COUNT = 4;

	class SubTexture
	{
	public:
		explicit SubTexture(class Texture* texture);
		explicit SubTexture(class Texture* texture, const Vec2& minUV,
			const Vec2& maxUV);
		~SubTexture();

		bool HasDefaultUVS() const;
		const Vec2* GetUVS() const;
		const Vec2& GetMinUV() const;
		const Vec2& GetMaxUV() const;

		const Vec2& GetSize() const;

		class Texture* GetTexture() const;

		static SubTexture* CreateFromTexCoords(class Texture* texture,
			const Vec2& minTextureCoord, const Vec2& maxTextureCoord);
		static SubTexture* CreateFromSizeAndTexCoord(class Texture* texture,
			const Vec2& texturePos, const Vec2& size);
		static SubTexture* CreateFromTextureAtlas(class Texture* texture,
			const Vec2& cellSize, const Vec2Int& atlasRowCol, const Vec2& scale = Vec2::One);

	private:
		class Texture* m_texture;
		Vec2 m_uvs[UV_COUNT];
		Vec2 m_size;
		bool m_defaultUVs;
	};
}