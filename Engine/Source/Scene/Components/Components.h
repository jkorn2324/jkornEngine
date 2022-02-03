#pragma once

#include "Source\Transform.h"

#include "Entity.h"
#include "BehaviorScriptContainer.h"
#include "SceneCamera.h"
#include "EntityHierarchyComponent.h"

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

	class BehaviorComponent
	{
	public:
		BehaviorComponent()
			: m_behaviorScriptContainer() { }
		BehaviorComponent(const BehaviorComponent& component)
			: m_behaviorScriptContainer() 
		{
			if (component.IsValid())
			{
				m_behaviorScriptContainer = std::unique_ptr<BehaviorScriptContainer>(
					new BehaviorScriptContainer(component.Get()));
			}
		}
		
		BehaviorComponent& operator=(const BehaviorComponent& component)
		{
			if (component.IsValid())
			{
				m_behaviorScriptContainer = std::unique_ptr<BehaviorScriptContainer>(
					new BehaviorScriptContainer(component.Get()));
			}
			return *this;
		}

		bool IsValid() const { return m_behaviorScriptContainer.get() != nullptr; }
		BehaviorScriptContainer& Get() { return *m_behaviorScriptContainer.get(); }
		const BehaviorScriptContainer& Get() const { return *m_behaviorScriptContainer.get(); }

	private:
		void Create(Entity& entity)
		{
			m_behaviorScriptContainer = std::make_unique<BehaviorScriptContainer>(entity);
		}

		void Destroy()
		{
			m_behaviorScriptContainer.release();
		}

	private:
		std::unique_ptr<BehaviorScriptContainer> m_behaviorScriptContainer;

		friend class Scene;
	};
}