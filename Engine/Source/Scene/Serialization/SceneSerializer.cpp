#include "EnginePCH.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "EntityRef.h"
#include "Components.h"
#include "Scene.h"

#include "JsonUtils.h"
#include "JsonFileReader.h"
#include "JsonFileWriter.h"

#include "Profiler.h"

namespace Engine
{

	template<typename TRegistry = entt::registry>
	static void SerializeEntity(JsonFileWriter& fileWriter,
		Entity& entity, TRegistry& registry)
	{
		TEntityRef<TRegistry> entityRef(entity, registry);
		std::vector<Entity> children;
		{
			PROFILE_SCOPE(SerializeEntity, Serialization);

			fileWriter.BeginObject();

			// UUID Component
			if (entityRef.DECLTDEPNAME HasComponent<IDComponent>())
			{
				IDComponent& component = entityRef.DECLTDEPNAME GetComponent<IDComponent>();
				fileWriter.Write("GUID", component.guid);
			}
			else
			{
				fileWriter.Write("GUID", (uint64_t)0);
			}

			// Name Component.
			if (entityRef.DECLTDEPNAME HasComponent<NameComponent>())
			{
				NameComponent& nameComponent = entityRef.DECLTDEPNAME GetComponent<NameComponent>();
				fileWriter.Write("NameComponent", nameComponent.name);
			}
			else
			{
				fileWriter.Write("NameComponent", (std::string)"");
			}

			// Transform 3D Component.
			if (entityRef.DECLTDEPNAME HasComponent<Transform3DComponent>())
			{
				Transform3DComponent& transform3D =
					entityRef.DECLTDEPNAME GetComponent<Transform3DComponent>();

				fileWriter.BeginObject("Transform3DComponent");
				fileWriter.Write("Position", transform3D.GetLocalPosition());
				fileWriter.Write("Rotation", transform3D.GetLocalRotation());
				fileWriter.Write("Scale", transform3D.GetLocalScale());
				fileWriter.EndObject();
			}

			// Transform 2D Component.
			if (entityRef.DECLTDEPNAME HasComponent<Transform2DComponent>())
			{
				Transform2DComponent& transform2D =
					entityRef.DECLTDEPNAME GetComponent<Transform2DComponent>();

				fileWriter.BeginObject("Transform2DComponent");
				fileWriter.Write("Position", transform2D.GetLocalPosition());
				fileWriter.Write("Rotation", transform2D.GetLocalRotation());
				fileWriter.Write("Scale", transform2D.GetLocalScale());
				fileWriter.EndObject();
			}

			// Sprite Component.
			if (entityRef.DECLTDEPNAME HasComponent<SpriteComponent>())
			{
				SpriteComponent& spriteComponent =
					entityRef.DECLTDEPNAME GetComponent<SpriteComponent>();

				fileWriter.BeginObject("SpriteComponent");

				if (spriteComponent.texture)
				{
					GUID guid;
					// TODO: Get GUID from Asset
					fileWriter.Write("Texture", (uint64_t)guid);
				}
				else
				{
					fileWriter.Write("Texture", 0);
				}

				fileWriter.Write("Enabled", spriteComponent.enabled);
				fileWriter.Write("Color", spriteComponent.color);
				fileWriter.EndObject();
			}

			// Mesh Component.
			if (entityRef.DECLTDEPNAME HasComponent<MeshComponent>())
			{
				MeshComponent& meshComponent =
					entityRef.DECLTDEPNAME GetComponent<MeshComponent>();

				fileWriter.BeginObject("MeshComponent");

				if (meshComponent.material)
				{
					GUID guid;
					// TODO: Get GUID from Asset
					fileWriter.Write("Material", (uint64_t)guid);
				}
				else
				{
					fileWriter.Write("Material", 0);
				}
				// TODO: Write Mesh
				fileWriter.Write("Enabled", meshComponent.enabled);
				fileWriter.EndObject();
			}

			// Scene Camera Component.
			if (entityRef.DECLTDEPNAME HasComponent<SceneCameraComponent>())
			{
				SceneCameraComponent& cameraComponent =
					entityRef.DECLTDEPNAME GetComponent<SceneCameraComponent>();
				CameraProperties& properties =
					cameraComponent.camera.GetProperties();

				fileWriter.BeginObject("SceneCameraComponent");
				fileWriter.Write("MainCamera", cameraComponent.mainCamera);
				fileWriter.Write("Enabled", cameraComponent.enabled);
				fileWriter.Write("CameraType", (uint32_t)cameraComponent.camera.GetSceneCameraType());
				fileWriter.Write("NearPlane", properties.nearPlane);
				fileWriter.Write("FarPlane", properties.farPlane);
				fileWriter.Write("PerspFOV", properties.perspFOV);
				fileWriter.Write("PerspAspectRatio", properties.perspAspectRatio);
				fileWriter.Write("OrthoSize", properties.perspAspectRatio);
				fileWriter.Write("OrthoWidth", properties.orthoWidth);
				fileWriter.Write("OrthoHeight", properties.orthoHeight);
				fileWriter.EndObject();
			}

			// Directional Light Component
			if (entityRef.DECLTDEPNAME HasComponent<DirectionalLightComponent>())
			{
				DirectionalLightComponent& component
					= entityRef.DECLTDEPNAME GetComponent<DirectionalLightComponent>();

				fileWriter.BeginObject("DirectionalLightComponent");
				fileWriter.Write("LightColor", component.lightColor);
				fileWriter.Write("LightIntensity", component.lightIntensity);
				fileWriter.Write("Enabled", component.enabled);
				fileWriter.EndObject();
			}

			// Point Light Component
			if (entityRef.DECLTDEPNAME HasComponent<PointLightComponent>())
			{
				PointLightComponent& component
					= entityRef.DECLTDEPNAME GetComponent<PointLightComponent>();

				fileWriter.BeginObject("PointLightComponent");
				fileWriter.Write("LightColor", component.lightColor);
				fileWriter.Write("InnerRadius", component.innerRadius);
				fileWriter.Write("OuterRadius", component.outerRadius);
				fileWriter.Write("LightIntensity", component.lightIntensity);
				fileWriter.Write("Enabled", component.enabled);
				fileWriter.EndObject();
			}

			if (entityRef.DECLTDEPNAME HasComponent<EntityHierarchyComponent>())
			{
				EntityHierarchyComponent& ehc = entityRef.DECLTDEPNAME GetComponent<EntityHierarchyComponent>();
				fileWriter.BeginObject("EntityHierarchyComponent");

				// Write the parent entity component.
				if (ehc.HasParent(registry))
				{
					const Entity& entity = ehc.GetParent();
					TEntityRef<TRegistry> ref(entity, registry);
					fileWriter.Write("Parent", (uint64_t)ref.DECLTDEPNAME GetComponent<IDComponent>().guid);
				}
				else
				{
					fileWriter.Write("Parent", 0);
				}
				children = ehc.GetChildren();
				fileWriter.EndObject();
			}
			fileWriter.EndObject();
		}

		// Serializes the entity's children.
		if (children.size() > 0)
		{
			for (auto e : children)
			{
				if (e.IsValid(registry)) SerializeEntity(fileWriter, e, registry);
			}
		}
	}


	template<typename TRegistry = entt::registry>
	static void DeserializeEntity(rapidjson::Value& value, Entity& entity, TRegistry& registry)
	{
		PROFILE_SCOPE(DeserializeEntity, Serialization);

		TEntityRef<TRegistry> entityRef(entity, registry);

		// UUID Component.
		GUID guid;
		if (ReadUint64(value, "GUID", *(uint64_t*)&guid))
		{
			if (entityRef.DECLTDEPNAME HasComponent<IDComponent>())
			{
				IDComponent& component = entityRef.DECLTDEPNAME GetComponent<IDComponent>();
				component.guid = guid;
			}
		}

		// Name Component.
		std::string name;
		if (ReadString(value, "NameComponent", name))
		{
			if (entityRef.DECLTDEPNAME HasComponent<NameComponent>())
			{
				NameComponent& component = entityRef.DECLTDEPNAME GetComponent<NameComponent>();
				component.name = name;
			}
			else
			{
				entityRef.DECLTDEPNAME AddComponent<NameComponent>(name);
			}
		}

		// Transform 3D Component.
		if (value.HasMember("Transform3DComponent"))
		{
			MathLib::Vector3 position;
			MathLib::Quaternion rotation;
			MathLib::Vector3 scale;

			ReadVector3(value["Transform3DComponent"],
				"Position", position);
			ReadQuaternion(value["Transform3DComponent"],
				"Rotation", rotation);
			ReadVector3(value["Transform3DComponent"],
				"Scale", scale);

			entityRef.DECLTDEPNAME AddComponent<Transform3DComponent>(position,
				rotation, scale);
		}

		// Transform 2D Component.
		if (value.HasMember("Transform2DComponent"))
		{
			MathLib::Vector2 position;
			float rotation;
			MathLib::Vector2 scale;

			ReadVector2(value["Transform2DComponent"],
				"Position", position);
			ReadFloat(value["Transform2DComponent"],
				"Rotation", rotation);
			ReadVector2(value["Transform2DComponent"],
				"Scale", scale);

			entityRef.DECLTDEPNAME AddComponent<Transform2DComponent>(position,
				rotation, scale);
		}

		// Sprite Component.
		if (value.HasMember("SpriteComponent"))
		{
			bool enabled = true;
			MathLib::Vector4 color;

			ReadBool(value["SpriteComponent"],
				"Enabled", enabled);
			ReadVector4(value["SpriteComponent"],
				"Color", color);

			entityRef.DECLTDEPNAME AddComponent<SpriteComponent>(enabled, color);
		}

		// Mesh Component
		if (value.HasMember("MeshComponent"))
		{
			// TODO: Load a mesh.

			MeshComponent& meshComponent
				= entityRef.DECLTDEPNAME AddComponent<MeshComponent>();
			uint64_t materialGUID;

			ReadBool(value["MeshComponent"], "Enabled", meshComponent.enabled);
			ReadUint64(value["MeshComponent"], "Material", materialGUID);

			// TODO: Load Material
		}

		// Scene Camera Component.
		if (value.HasMember("SceneCameraComponent"))
		{
			bool enabled = true;
			bool mainCamera = false;
			uint32_t cameraType;
			CameraProperties cameraProperties;

			ReadBool(value["SceneCameraComponent"],
				"MainCamera", mainCamera);
			ReadBool(value["SceneCameraComponent"],
				"Enabled", enabled);
			ReadUint32(value["SceneCameraComponent"],
				"CameraType", cameraType);
			ReadFloat(value["SceneCameraComponent"],
				"NearPlane", cameraProperties.nearPlane);
			ReadFloat(value["SceneCameraComponent"],
				"FarPlane", cameraProperties.farPlane);
			ReadFloat(value["SceneCameraComponent"],
				"PerspFOV", cameraProperties.perspFOV);
			ReadFloat(value["SceneCameraComponent"],
				"PerspAspectRatio", cameraProperties.perspAspectRatio);
			ReadFloat(value["SceneCameraComponent"],
				"OrthoSize", cameraProperties.orthoSize);
			ReadFloat(value["SceneCameraComponent"],
				"OrthoWidth", cameraProperties.orthoWidth);
			ReadFloat(value["SceneCameraComponent"],
				"OrthoHeight", cameraProperties.orthoHeight);

			entityRef.DECLTDEPNAME AddComponent<SceneCameraComponent>(mainCamera,
				(SceneCameraType)cameraType, cameraProperties);
		}

		// Directional Light Component.
		if (value.HasMember("DirectionalLightComponent"))
		{
			DirectionalLightComponent& component =
				entityRef.DECLTDEPNAME AddComponent<DirectionalLightComponent>();
			ReadVector3(value["DirectionalLightComponent"], "LightColor", component.lightColor);
			ReadFloat(value["DirectionalLightComponent"], "LightIntensity", component.lightIntensity);
			ReadBool(value["DirectionalLightComponent"], "Enabled", component.enabled);
		}

		// Point Light Component
		if (value.HasMember("PointLightComponent"))
		{
			PointLightComponent& component =
				entityRef.DECLTDEPNAME AddComponent<PointLightComponent>();
			ReadVector3(value["PointLightComponent"], "LightColor", component.lightColor);
			ReadFloat(value["PointLightComponent"], "InnerRadius", component.innerRadius);
			ReadFloat(value["PointLightComponent"], "OuterRadius", component.outerRadius);
			ReadFloat(value["PointLightComponent"], "LightIntensity", component.lightIntensity);
			ReadBool(value["PointLightComponent"], "Enabled", component.enabled);
		}

		// Entity Hierarchy Component.
		if (value.HasMember("EntityHierarchyComponent"))
		{
			rapidjson::Value& hierarchyComponent = value["EntityHierarchyComponent"].GetObject();
			uint64_t entityOwnerID;
			ReadUint64(hierarchyComponent, "Parent", entityOwnerID);

			if (entityRef.DECLTDEPNAME HasComponent<EntityHierarchyComponent>())
			{
				EntityHierarchyComponent& component
					= entityRef.DECLTDEPNAME GetComponent<EntityHierarchyComponent>();
				if (entityOwnerID != 0)
				{
					TEntityRef<TRegistry> eRef(entityOwnerID, registry);
					if (eRef.IsValid()) component.SetParent(eRef);
				}
			}
			else
			{
				EntityHierarchyComponent& component
					= entityRef.DECLTDEPNAME AddComponent<EntityHierarchyComponent>(entity);
				if (entityOwnerID != 0)
				{
					TEntityRef<TRegistry> eRef(entityOwnerID, registry);
					if (eRef.IsValid()) component.SetParent(eRef);
				}
			}
		}
	}


	SceneSerializer::SceneSerializer(Scene* scene)
		: m_scene(scene) { }


	void SceneSerializer::Serialize(const std::filesystem::path& filePath)
	{
		PROFILE_SCOPE(SerializeScene, Serialization);

		JsonFileWriter jsonFileWriter(filePath);
		jsonFileWriter.BeginArray("Entities");

		auto view = m_scene->m_entityRegistry.view<entt::entity>();
		view.each([&](auto id)
			{
				entt::registry& registry = m_scene->m_entityRegistry;

				Entity entity(id);
				if (!entity.IsValid(registry))
				{
					return;
				}
				SerializeEntity(jsonFileWriter, entity, registry);
			});
		jsonFileWriter.EndArray();
		jsonFileWriter.Flush();
	}

	void SceneSerializer::Deserialize(const std::filesystem::path& filePath)
	{
		PROFILE_SCOPE(DeserializeScene, Serialization);

		JsonFileReader parser = { filePath };
		Deserialize(parser);
	}

	void SceneSerializer::Deserialize(const JsonFileReader& jsonParser)
	{
		if (jsonParser.IsValid())
		{
			rapidjson::Document& document = (rapidjson::Document&)jsonParser.GetDocument();

			if (document.HasMember("Entities")
				&& document["Entities"].IsArray())
			{
				auto entitiesArray = document["Entities"].GetArray();
				for (auto value = entitiesArray.begin();
					value != entitiesArray.end(); value++)
				{
					Entity entity = m_scene->CreateEntity();
					DeserializeEntity(*value, entity, m_scene->m_entityRegistry);
				}
			}
		}
	}
}
