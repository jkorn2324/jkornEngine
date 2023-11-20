#pragma once

#include "Vector.h"
#include "MathLib.h"

namespace Engine
{
	using Vec2 = MathLib::Vector2;
	using Vec2Int = MathLib::Vector2Int;

	const uint32_t UV_COUNT = 4;

	class Texture;

	class SubTextureContext
	{
	public:
		SubTextureContext(float width, float height);
		SubTextureContext(float width, float height, const Vec2& minUV,
			const Vec2& maxUV);

		bool HasDefaultUVS() const;
		const Vec2* GetUVS() const;
		const Vec2& GetMinUV() const;
		const Vec2& GetMaxUV() const;

		const Vec2& GetSize() const;

		static SubTextureContext CreateFromTexCoords(Texture* texture,
			const Vec2& minTextureCoord, const Vec2& maxTextureCoord);
		static SubTextureContext CreateFromSizeAndTexCoord(Texture* texture,
			const Vec2& texturePos, const Vec2& size);
		static SubTextureContext CreateFromTextureAtlas(Texture* texture,
			const Vec2& cellSize, const Vec2Int& atlasRowCol, const Vec2& scale = Vec2::One);

	private:
		Vec2 m_uvs[UV_COUNT];
		Vec2 m_size;
	};

	/**
	 * The sub texture context. 
	 */
	struct SubTexture
	{
		SubTextureContext subTextureContext;
		Texture* texture = nullptr;

		SubTexture()
			: texture(nullptr), subTextureContext(0.0f, 0.0f) { }
		SubTexture(Texture* texture, const SubTextureContext& context)
			: texture(texture), subTextureContext(context) { }
	};
}
