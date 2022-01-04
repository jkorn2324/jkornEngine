#pragma once

#include "Source\Transform.h"

#include "Entity.h"
#include "SceneCamera.h"
#include "MeshComponent.h"

namespace Engine
{
	using Transform2DComponent = MathLib::Transform2D;
	using Transform3DComponent = MathLib::Transform3D;

	struct SpriteComponent
	{
		class Texture* texture = nullptr;
		MathLib::Vector4 color = MathLib::Vector4::One;
		bool enabled;

		explicit SpriteComponent() = default;
		explicit SpriteComponent(const MathLib::Vector4& color)
			: color(color), texture(nullptr), enabled(true) { }
		explicit SpriteComponent(class Texture* texture, const MathLib::Vector4& color)
			: color(color), texture(texture), enabled(true) { }
		explicit SpriteComponent(class Texture* texture)
			: color(MathLib::Vector4::One), texture(texture), enabled(true) { }
		explicit SpriteComponent(bool enabled)
			: color(MathLib::Vector4::One), texture(nullptr), enabled(enabled) { }
	};

	struct SceneCameraComponent
	{
		bool mainCamera = true;
		SceneCamera camera;

		explicit SceneCameraComponent() = default;
		explicit SceneCameraComponent(bool mainCam)
			: mainCamera(mainCam), camera() { }
		SceneCameraComponent(const SceneCameraComponent& component) = default;
	};
}