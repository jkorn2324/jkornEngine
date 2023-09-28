#pragma once

#include "Transform.h"

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

		IDComponent(const IDComponent& cpy)
			: guid(cpy.guid) { }

		IDComponent& operator=(const IDComponent& cpy)
		{
			guid = cpy.guid;
			return *this;
		}
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

		SpriteComponent(const SpriteComponent& cpy)
			: color(cpy.color), texture(cpy.texture), enabled(cpy.enabled) { }

		SpriteComponent& operator=(const SpriteComponent& cpy)
		{
			texture = cpy.texture;
			color = cpy.color;
			enabled = cpy.enabled;
			return *this;
		}
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

		SceneCameraComponent(const SceneCameraComponent& component)
			: camera(component.camera), mainCamera(component.mainCamera), enabled(component.enabled)
		{
		}

		SceneCameraComponent& operator=(const SceneCameraComponent& cpy)
		{
			camera = cpy.camera;
			mainCamera = cpy.mainCamera;
			enabled = cpy.enabled;
			return *this;
		}
	};

	struct NameComponent
	{
		std::string name;

		explicit NameComponent() : name() { }
		explicit NameComponent(const std::string& name)
			: name(name) { }
		explicit NameComponent(const char* name)
			: name(name) { }

		NameComponent(const NameComponent& component)
			: name(component.name)
		{

		}

		NameComponent& operator=(const NameComponent& cpy)
		{
			name = cpy.name;
			return *this;
		}
	};

	struct MeshComponent
	{
		// TODO: Generate a default material.

		AssetRef<Mesh> mesh;
		AssetRef<Material> material;
		bool enabled = true;

		explicit MeshComponent()
			: mesh(), material() { }
		explicit MeshComponent(const AssetRef<Mesh>& mesh, const AssetRef<Material>& material)
			: mesh(mesh), material(material) { }

		MeshComponent(const MeshComponent& mesh)
			: mesh(mesh.mesh), material(mesh.material), enabled(mesh.enabled) { }

		MeshComponent& operator=(const MeshComponent& cpy)
		{
			mesh = cpy.mesh;
			material = cpy.material;
			enabled = cpy.enabled;
			return *this;
		}
	};
}
