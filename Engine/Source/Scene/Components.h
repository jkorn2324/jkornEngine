#pragma once

#include "Source\Transform.h"
#include "Entity.h"

namespace Engine
{
	using Transform2DComponent = MathLib::Transform2D;
	using Transform3DComponent = MathLib::Transform3D;

	struct SpriteComponent
	{
		// TODO: More options

		class Texture* texture = nullptr;
		MathLib::Vector4 color;

		explicit SpriteComponent() = default;
		explicit SpriteComponent(Texture* texture, const MathLib::Vector4& color)
			: color(color), texture(texture) { }
	};
}