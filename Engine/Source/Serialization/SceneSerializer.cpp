#include "EnginePCH.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"
#include "Scene.h"

#include "JsonUtils.h"
#include "JsonFileParser.h"
#include "JsonFileWriter.h"

#include "AssetManager.h"
#include "AssetMapper.h"

#include "Profiler.h"

namespace Engine
{

	static void SerializeEntity(JsonFileWriter& fileWriter,
		Entity& entity)
	{
		std::vector<Entity> children;
		{
			PROFILE_SCOPE(SerializeEntity, Serialization);

			fileWriter.BeginObject();

			// UUID Component
			if (entity.HasComponent<IDComponent>())
			{
				IDComponent& component = entity.GetComponent<IDComponent>();
				fileWriter.Write("GUID", component.guid);
			}
			else
			{
				fileWriter.Write("GUID", (uint64_t)0);
			}

			// Name Component.
			if (entity.HasComponent<NameComponent>())
			{
				NameComponent& nameComponent = entity.GetComponent<NameComponent>();
				fileWriter.Write("NameComponent", nameComponent.name);
			}
			else
			{
				fileWriter.Write("NameComponent", (std::string)"");
			}

			// Transform 3D Component.
			if (entity.HasComponent<Transform3DComponent>())
			{
				Transform3DComponent& transform3D =
					entity.GetComponent<Transform3DComponent>();

				fileWriter.BeginObject("Transform3DComponent");
				fileWriter.Write("Position", transform3D.GetLocalPosition());
				fileWriter.Write("Rotation", transform3D.GetLocalRotation());
				fileWriter.Write("Scale", transform3D.GetLocalScale());
				fileWriter.EndObject();
			}

			// Transform 2D Component.
			if (entity.HasComponent<Transform2DComponent>())
			{
				Transform2DComponent& transform2D =
					entity.GetComponent<Transform2DComponent>();

				fileWriter.BeginObject("Transform2DComponent");
				fileWriter.Write("Position", transform2D.GetLocalPosition());
				fileWriter.Write("Rotation", transform2D.GetLocalRotation());
				fileWriter.Write("Scale", transform2D.GetLocalScale());
				fileWriter.EndObject();
			}

			// Sprite Component.
			if (entity.HasComponent<SpriteComponent>())
			{
				SpriteComponent& spriteComponent =
					entity.GetComponent<SpriteComponent>();

				fileWriter.BeginObject("SpriteComponent");

				if (spriteComponent.texture)
				{
					GUID guid;
					spriteComponent.texture.GetGUID(guid);
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
			if (entity.HasComponent<MeshComponent>())
			{
				MeshComponent& meshComponent =
					entity.GetComponent<MeshComponent>();

				fileWriter.BeginObject("MeshComponent");

				if (meshComponent.material)
				{
					GUID guid;
					meshComponent.material.GetGUID(guid);
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
			if (entity.HasComponent<SceneCameraComponent>())
			{
				SceneCameraComponent& cameraComponent =
					entity.GetComponent<SceneCameraComponent>();
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
			if (entity.HasComponent<DirectionalLightComponent>())
			{
				DirectionalLightComponent& component
					= entity.GetComponent<DirectionalLightComponent>();

				fileWriter.BeginObject("DirectionalLightComponent");
				fileWriter.Write("LightColor", component.lightColor);
				fileWriter.Write("LightIntensity", component.lightIntensity);
				fileWriter.Write("Enabled", component.enabled);
				fileWriter.EndObject();
			}

			// Point Light Component
			if (entity.HasComponent<PointLightComponent>())
			{
				PointLightComponent& component
					= entity.GetComponent<PointLightComponent>();

				fileWriter.BeginObject("PointLightComponent");
				fileWriter.Write("LightColor", component.lightColor);
				fileWriter.Write("InnerRadius", component.innerRadius);
				fileWriter.Write("OuterRadius", component.outerRadius);
				fileWriter.Write("LightIntensity", component.lightIntensity);
				fileWriter.Write("Enabled", component.enabled);
				fileWriter.EndObject();
			}

			if (entity.HasComponent<EntityHierarchyComponent>())
			{
				EntityHierarchyComponent& ehc = entity.GetComponent<EntityHierarchyComponent>();
				fileWriter.BeginObject("EntityHierarchyComponent");

				// Write the parent entity component.
				if (ehc.HasParent())
				{
					const Entity& entity = ehc.GetParent();
					fileWriter.Write("Parent", (uint64_t)entity.GetComponent<IDComponent>().guid);
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
				if (e.IsValid()) SerializeEntity(fileWriter, e);
			}
		}
	}


	static void DeserializeEntity(rapidjson::Value& value, Entity& entity)
	{
		PROFILE_SCOPE(DeserializeEntity, Serialization);

		// UUID Component.
		GUID guid;
		if (ReadUint64(value, "GUID", *(uint64_t*)&guid))
		{
			if (entity.HasComponent<IDComponent>())
			{
				IDComponent& component = entity.GetComponent<IDComponent>();
				component.guid = guid;
			}
		}

		// Name Component.
		std::string name;
		if (ReadString(value, "NameComponent", name))
		{
			if (entity.HasComponent<NameComponent>())
			{
				NameComponent& component = entity.GetComponent<NameComponent>();
				component.name = name;
			}
			else
			{
				entity.AddComponent<NameComponent>(name);
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

			entity.AddComponent<Transform3DComponent>(position,
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

			entity.AddComponent<Transform2DComponent>(position,
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

			entity.AddComponent<SpriteComponent>(enabled, color);
		}

		// Mesh Component
		if (value.HasMember("MeshComponent"))
		{
			// TODO: Load a mesh.

			MeshComponent& meshComponent 
				= entity.AddComponent<MeshComponent>();
			uint64_t materialGUID;

			ReadBool(value["MeshComponent"], "Enabled", meshComponent.enabled);
			ReadUint64(value["MeshComponent"], "Material", materialGUID);

			AssetManager::GetMaterials().Load(meshComponent.material,
				AssetManager::GetAssetMapper().GetPath(GUID(materialGUID)));
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

			entity.AddComponent<SceneCameraComponent>(mainCamera,
				(SceneCameraType)cameraType, cameraProperties);
		}

		// Directional Light Component.
		if (value.HasMember("DirectionalLightComponent"))
		{
			DirectionalLightComponent& component =
				entity.AddComponent<DirectionalLightComponent>();
			ReadVector3(value["DirectionalLightComponent"], "LightColor", component.lightColor);
			ReadFloat(value["DirectionalLightComponent"], "LightIntensity", component.lightIntensity);
			ReadBool(value["DirectionalLightComponent"], "Enabled", component.enabled);
		}

		// Point Light Component
		if (value.HasMember("PointLightComponent"))
		{
			PointLightComponent& component =
				entity.AddComponent<PointLightComponent>();
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

			if (entity.HasComponent<EntityHierarchyComponent>())
			{
				EntityHierarchyComponent& component
					= entity.GetComponent<EntityHierarchyComponent>();
				if (entityOwnerID != 0)
				{
					Entity e = entity.GetScene().Find(entityOwnerID);
					if (e.IsValid()) component.SetParent(e);
				}
			}
			else
			{
				EntityHierarchyComponent& component
					= entity.AddComponent<EntityHierarchyComponent>(entity);
				if (entityOwnerID != 0)
				{
					Entity e = entity.GetScene().Find(entityOwnerID);
					if (e.IsValid()) component.SetParent(e);
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

		m_scene->m_entityRegistry.each([&](auto id)
			{
				Entity entity(id, m_scene);
				if (!entity.IsValid())
				{
					return;
				}
				SerializeEntity(jsonFileWriter, entity);
			});

		jsonFileWriter.EndArray();
		jsonFileWriter.Flush();
	}

	void SceneSerializer::Deserialize(const std::filesystem::path& filePath)
	{
		PROFILE_SCOPE(DeserializeScene, Serialization);

		JsonFileParser parser = { filePath };
		Deserialize(parser);
	}

	void SceneSerializer::Deserialize(const JsonFileParser& jsonParser)
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
					DeserializeEntity(*value, entity);
				}
			}
		}
	}
}