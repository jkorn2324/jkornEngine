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
		MathLib::Vector4 color = MathLib::Vector4::One;
		bool enabled = true;

		explicit SpriteComponent() = default;
		explicit SpriteComponent(const MathLib::Vector4& color)
			: color(color), texture(nullptr), enabled(true) { }
		explicit SpriteComponent(class Texture* texture, const MathLib::Vector4& color)
			: color(color), texture(texture), enabled(true) { }
		explicit SpriteComponent(class Texture* texture)
			: color(MathLib::Vector4::One), texture(texture), enabled(true) { }
		explicit SpriteComponent(bool enabled)
			: color(MathLib::Vector4::One), texture(nullptr), enabled(enabled) { }
		explicit SpriteComponent(bool enabled, const MathLib::Vector4& color)
			: color(color), enabled(enabled), texture(nullptr) { }
	};

	struct SceneCameraComponent
	{
		SceneCamera camera;
		bool mainCamera = true;
		bool enabled = true;

		explicit SceneCameraComponent() = default;
		explicit SceneCameraComponent(bool mainCam)
			: mainCamera(mainCam), camera() { }
		SceneCameraComponent(const SceneCameraComponent& component) = default;
		SceneCameraComponent(bool mainCamera, SceneCameraType type,
			const CameraProperties& properties)
			: mainCamera(mainCamera), camera(type, properties) { }
	};

	struct NameComponent
	{
		std::string name;

		explicit NameComponent(const std::string& name)
			: name(name) { }
		explicit NameComponent(const char* name)
			: name(name) { }
	};

	struct MeshComponent
	{
		// TODO: Generate a default material.

		bool enabled = true;
		class Mesh* mesh;
		class Material* material;

		explicit MeshComponent()
			: mesh(nullptr), material(nullptr) { }
		explicit MeshComponent(class Mesh* mesh, class Material* material)
			: mesh(mesh), material(material) { }
	};
}