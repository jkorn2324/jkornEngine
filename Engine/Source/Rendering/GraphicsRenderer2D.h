#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace Engine
{
	template<typename T>
	class AssetRef;
	class Texture;
	class SubTexture;

	class GraphicsRenderer2D
	{
	public:
		static void Init();
		static void Release();

		static void DrawRect(const MathLib::Vector2& pos,
			const MathLib::Vector2& scale, const AssetRef<Texture>& texture,
			int32_t entityID = -1);
		static void DrawRect(const MathLib::Matrix4x4& transformMat,
			const MathLib::Vector4& color, const AssetRef<Texture>& texture, 
			int32_t entityID = -1);
		static void DrawRect(const MathLib::Vector2& pos,
			const MathLib::Vector2& scale, SubTexture* texture, int32_t entityID = -1);
		static void DrawRect(const MathLib::Matrix4x4& transformMat,
			const MathLib::Vector4& color, SubTexture* texture, int32_t entityID = -1);
	};
}
