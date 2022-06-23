#pragma once

#include "Source\Transform.h"

#include "Entity.h"
#include "GUID.h"
#include "BehaviorScriptContainer.h"
#include "SceneCamera.h"
#include "EntityHierarchyComponent.h"
#include "LightingComponents.h"
#include "BehaviorScript.h"

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
		Texture* texture;
		MathLib::Vector4 color = MathLib::Vector4::One;
		bool enabled = true;

		explicit SpriteComponent() = default;
		explicit SpriteComponent(const MathLib::Vector4& color)
			: color(color), texture(), enabled(true) { }
		explicit SpriteComponent(Texture* texture, const MathLib::Vector4& color)
			: color(color), texture(texture), enabled(true) { }
		explicit SpriteComponent(Texture* texture)
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
		Mesh* mesh;
		Material* material;

		explicit MeshComponent()
			: mesh(), material() { }
		explicit MeshComponent(Mesh* mesh, Material* material)
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
				m_behaviorScriptContainer = component.m_behaviorScriptContainer;
			}
		}
		
		BehaviorComponent& operator=(const BehaviorComponent& component)
		{
			if (component.IsValid())
			{
				m_behaviorScriptContainer = component.m_behaviorScriptContainer;
			}
			return *this;
		}

		bool IsValid() const { return m_behaviorScriptContainer.get() != nullptr; }
		BehaviorScriptContainer& Get() { return *m_behaviorScriptContainer.get(); }
		const BehaviorScriptContainer& Get() const { return *m_behaviorScriptContainer.get(); }

		friend void Copy(const BehaviorComponent& from, BehaviorComponent& to)
		{
			to.Copy(from);
		}

	private:
		void Copy(const BehaviorComponent& from)
		{
			if (from.IsValid())
			{
				for (const auto& behavior : from.m_behaviorScriptContainer->GetBehaviors())
				{
					m_behaviorScriptContainer->CopyBehavior(behavior);
				}
			}
		}

		void Create(Entity& entity)
		{
			m_behaviorScriptContainer = std::make_shared<BehaviorScriptContainer>(entity);
		}

		void Destroy()
		{
			m_behaviorScriptContainer->Deallocate();
		}

	private:
		std::shared_ptr<BehaviorScriptContainer> m_behaviorScriptContainer;

		friend class Scene;
	};
}