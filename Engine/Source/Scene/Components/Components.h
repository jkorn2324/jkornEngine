#pragma once

#include "Source\Transform.h"

#include "Entity.h"
#include "GUID.h"
#include "SceneCamera.h"
#include "EntityHierarchyComponent.h"
#include "LightingComponents.h"
#include "AssetReferenceManager.h"

namespace Engine
{
	using Transform2DComponent = MathLib::Transform2D;
	using Transform3DComponent = MathLib::Transform3D;

	class Mesh;
	class Texture;
	class Material;
	class Shader;

	struct IDComponent
	{
		GUID guid;

		IDComponent() = default;
		IDComponent(const GUID& guid)
			: guid(guid) { }
	};

	struct SpriteComponent
	{
		AssetRef<Texture> texture;
		MathLib::Vector4 color = MathLib::Vector4::One;
		bool enabled = true;

		explicit SpriteComponent() = default;
		explicit SpriteComponent(const MathLib::Vector4& color)
			: color(color), texture(), enabled(true) { }
		explicit SpriteComponent(const AssetRef<Texture> texture, const MathLib::Vector4& color)
			: color(color), texture(texture), enabled(true) { }
		explicit SpriteComponent(const AssetRef<Texture> texture)
			: color(MathLib::Vector4::One), texture(texture), enabled(true) { }
		explicit SpriteComponent(bool enabled)
			: color(MathLib::Vector4::One), texture(), enabled(enabled) { }
		explicit SpriteComponent(bool enabled, const MathLib::Vector4& color)
			: color(color), enabled(enabled), texture() { }
	};

	struct SceneCameraComponent
	{
		SceneCamera camera;
		bool mainCamera = true;
		bool enabled = true;

		explicit SceneCameraComponent() = default;
		explicit SceneCameraComponent(bool mainCam)
			: mainCamera(mainCam), camera() { }
		explicit SceneCameraComponent(bool mainCam,
			SceneCameraType cameraType)
			: mainCamera(mainCam), enabled(true), camera()
		{
			camera.SetSceneCameraType(cameraType);
		}

		explicit SceneCameraComponent(bool mainCamera, SceneCameraType type,
			const CameraProperties& properties)
			: mainCamera(mainCamera), camera(type, properties) { }

		SceneCameraComponent(const SceneCameraComponent& component) = default;

	};

	struct NameComponent
	{
		std::string name;

		explicit NameComponent() : name() { }
		explicit NameComponent(const std::string& name)
			: name(name) { }
		explicit NameComponent(const char* name)
			: name(name) { }
	};

	struct MeshComponent
	{
		// TODO: Generate a default material.

		bool enabled = true;
		AssetRef<Mesh> mesh;
		AssetRef<Material> material;

		explicit MeshComponent()
			: mesh(), material() { }
		explicit MeshComponent(const AssetRef<Mesh>& mesh, const AssetRef<Material>& material)
			: mesh(mesh), material(material) { }
	};
}