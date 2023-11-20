#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Memory.h"

namespace Engine
{
	class Texture;
	class SubTextureContext;
	struct SubTexture;

	class GraphicsRenderer2D
	{
	public:
		static void Init();
		static void Release();

		static void DrawRect(const MathLib::Vector2& pos,
			const MathLib::Vector2& scale, Texture* texture,
			int32_t entityID = -1);
		static void DrawRect(const MathLib::Matrix4x4& transformMat,
			const MathLib::Vector4& color, Texture* texture, 
			int32_t entityID = -1);
		
		static void DrawRect(const MathLib::Vector2& pos,
			const MathLib::Vector2& scale, const SubTexture& subTexture, int32_t entityID = -1);

		static void DrawRect(const MathLib::Vector2& pos,
			const MathLib::Vector2& scale, Texture* texture, const SubTextureContext& context, int32_t entityID = -1);
		static void DrawRect(const MathLib::Matrix4x4& transformMat,
			const MathLib::Vector4& color, Texture* texture, const SubTextureContext& context, int32_t entityID = -1);
	};
}
