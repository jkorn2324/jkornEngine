#pragma once

#include "Source\Transform.h"
#include "Entity.h"
#include "SceneCamera.h"

namespace Engine
{

	using Transform2DComponent = MathLib::Transform2D;
	using Transform3DComponent = MathLib::Transform3D;

	struct SpriteComponent
	{
		class Texture* texture = nullptr;
		MathLib::Vector4 color;

		explicit SpriteComponent() = default;
		explicit SpriteComponent(const MathLib::Vector4& color)
			: color(color), texture(nullptr) { }
		explicit SpriteComponent(class Texture* texture, const MathLib::Vector4& color)
			: color(color), texture(texture) { }
	};

	struct SceneCameraComponent
	{
		bool mainCamera = true;
		SceneCamera camera;

		explicit SceneCameraComponent() = default;
		explicit SceneCameraComponent(bool mainCam)
			: mainCamera(mainCam) { }
		SceneCameraComponent(const SceneCameraComponent& component) = default;
	};
}