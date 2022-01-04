#pragma once

#include "Source\Vector.h"
#include "Source\Matrix.h"

namespace Engine
{

	class GraphicsRenderer2D
	{
	public:
		static void Init();
		static void Release();

		static void DrawRect(const MathLib::Vector2& pos,
			const MathLib::Vector2& scale, class Texture* texture);
		static void DrawRect(const MathLib::Matrix4x4& transformMat,
			const MathLib::Vector4& color, class Texture* texture);
	};
}