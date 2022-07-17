#pragma once

#include "Source\Vector.h"
#include "Source\Matrix.h"
#include "Memory.h"

namespace Engine
{
	class Texture;
	class SubTexture;

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
			const MathLib::Vector2& scale, SubTexture* texture, int32_t entityID = -1);
		static void DrawRect(const MathLib::Matrix4x4& transformMat,
			const MathLib::Vector4& color, SubTexture* texture, int32_t entityID = -1);
	};
}