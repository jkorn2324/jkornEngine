#include "EnginePCH.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"
#include "Scene.h"

#include "JsonUtils.h"
#include "JsonFileParser.h"
#include "Profiler.h"

namespace Engine
{

	static void SerializeEntity(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		Entity& entity)
	{
		PROFILE_SCOPE(SerializeEntity, Serialization);

		writer.StartObject();

		// UUID Component
		WriteString(writer, "Entity");
		WriteString(writer, "");

		// Name Component.
		if (entity.HasComponent<NameComponent>())
		{
			NameComponent& nameComponent = entity.GetComponent<NameComponent>();
			WriteString(writer, "NameComponent");
			WriteString(writer, nameComponent.name);
		}

		if (entity.HasComponent<EntityHierarchyComponent>())
		{
			EntityHierarchyComponent& ehc = entity.GetComponent<EntityHierarchyComponent>();
			// TODO: Write the parent Guid and Children Guid
		}

		// Transform 3D Component.
		if (entity.HasComponent<Transform3DComponent>())
		{
			Transform3DComponent& transform3D = 
				entity.GetComponent<Transform3DComponent>();

			WriteString(writer, "Transform3DComponent");
			writer.StartObject();

			WriteString(writer, "Position");
			WriteVector3(writer, transform3D.GetLocalPosition());
			WriteString(writer, "Rotation");
			WriteQuaternion(writer, transform3D.GetLocalRotation());
			WriteString(writer, "Scale");
			WriteVector3(writer, transform3D.GetLocalScale());

			writer.EndObject();
		}

		// Transform 2D Component.
		if (entity.HasComponent<Transform2DComponent>())
		{
			Transform2DComponent& transform2D =
				entity.GetComponent<Transform2DComponent>();

			WriteString(writer, "Transform2DComponent");
			writer.StartObject();

			WriteString(writer, "Position");
			WriteVector2(writer, transform2D.GetLocalPosition());
			WriteString(writer, "Rotation");
			writer.Double(transform2D.GetLocalRotation());
			WriteString(writer, "Scale");
			WriteVector2(writer, transform2D.GetLocalScale());

			writer.EndObject();
		}

		// Sprite Component.
		if (entity.HasComponent<SpriteComponent>())
		{
			SpriteComponent& spriteComponent =
				entity.GetComponent<SpriteComponent>();

			WriteString(writer, "SpriteComponent");
			writer.StartObject();

			WriteString(writer, "Enabled");
			writer.Bool(spriteComponent.enabled);
			WriteString(writer, "Color");
			WriteVector4(writer, spriteComponent.color);

			// TODO: Implement GUIDs for Assets

			writer.EndObject();
		}

		// Scene Camera Component.
		if (entity.HasComponent<SceneCameraComponent>())
		{
			SceneCameraComponent& cameraComponent =
				entity.GetComponent<SceneCameraComponent>();
			CameraProperties& properties = 
				cameraComponent.camera.GetProperties();

			WriteString(writer, "SceneCameraComponent");
			writer.StartObject();

			WriteString(writer, "MainCamera");
			writer.Bool(cameraComponent.mainCamera);
			
			WriteString(writer, "CameraType");
			writer.Uint((uint32_t)cameraComponent.camera.GetSceneCameraType());
			WriteString(writer, "NearPlane");
			writer.Double((double)properties.nearPlane);
			WriteString(writer, "FarPlane");
			writer.Double((double)properties.farPlane);
			WriteString(writer, "PerspFOV");
			writer.Double((double)properties.perspFOV);
			WriteString(writer, "PerspAspectRatio");
			writer.Double((double)properties.perspAspectRatio);
			WriteString(writer, "OrthoWidth");
			writer.Double((double)properties.orthoWidth);
			WriteString(writer, "OrthoHeight");
			writer.Double((double)properties.orthoHeight);

			writer.EndObject();
		}

		writer.EndObject();
	}


	static void DeserializeEntity(rapidjson::Value& value, Entity& entity)
	{
		PROFILE_SCOPE(DeserializeEntity, Serialization);

		// UUID Component.
		std::string uuid;
		ReadString(value, "UUID", uuid);

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

		if (value.HasMember("EntityHierarchyComponent"))
		{
			if (entity.HasComponent<EntityHierarchyComponent>())
			{
				// TODO: Load the owning entity's Guid.
				// TODO: Load the children entity Guids.
				EntityHierarchyComponent component 
					= entity.GetComponent<EntityHierarchyComponent>();
			}
			else
			{
				// TODO: Load the owning entity's Guid.
				// TODO: Load the children entity Guids.
				EntityHierarchyComponent component
					= entity.AddComponent<EntityHierarchyComponent>(entity);
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

		// Scene Camera Component.
		if (value.HasMember("SceneCameraComponent"))
		{
			bool mainCamera = false;
			uint32_t cameraType;
			CameraProperties cameraProperties;

			ReadBool(value["SceneCameraComponent"],
				"MainCamera", mainCamera);
			ReadUInt32(value["SceneCameraComponent"],
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
				"OrthoWidth", cameraProperties.orthoWidth);
			ReadFloat(value["SceneCameraComponent"],
				"OrthoHeight", cameraProperties.orthoHeight);

			entity.AddComponent<SceneCameraComponent>(mainCamera,
				(SceneCameraType)cameraType, cameraProperties);
		}
	}


	SceneSerializer::SceneSerializer(Scene* scene)
		: m_scene(scene) { }


	void SceneSerializer::Serialize(const std::wstring& filePath)
	{
		PROFILE_SCOPE(SerializeScene, Serialization);

		rapidjson::StringBuffer sceneBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(sceneBuffer);

		prettyWriter.StartObject();

		WriteString(prettyWriter, "Entities");
		prettyWriter.StartArray();

		m_scene->m_entityRegistry.each([&](auto id)
			{
				Entity entity(id, m_scene);
				if (!entity.IsValid())
				{
					return;
				}
				SerializeEntity(prettyWriter, entity);
			});

		prettyWriter.EndArray();
		prettyWriter.EndObject();

		FILE* file;
		_wfopen_s(&file, filePath.c_str(), L"w");
		DebugAssert(file != nullptr, "Something went wrong with serializing to a file.");
		fprintf_s(file, "%s", sceneBuffer.GetString());
		fclose(file);
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		PROFILE_SCOPE(SerializeScene, Serialization);

		rapidjson::StringBuffer sceneBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(sceneBuffer);

		prettyWriter.StartObject();

		WriteString(prettyWriter, "Entities");
		prettyWriter.StartArray();

		m_scene->m_entityRegistry.each([&](auto id)
		{
			Entity entity(id, m_scene);
			if (!entity.IsValid())
			{
				return;
			}
			SerializeEntity(prettyWriter, entity);
		});

		prettyWriter.EndArray();
		prettyWriter.EndObject();

		FILE* file;
		fopen_s(&file, filePath.c_str(), "w");
		DebugAssert(file != nullptr, "Something went wrong with serializing to a file.");
		fprintf_s(file, "%s", sceneBuffer.GetString());
		fclose(file);
	}


	void SceneSerializer::Deserialize(const std::wstring& filePath)
	{
		PROFILE_SCOPE(DeserializeScene, Serialization);

		JsonFileParser parser = { filePath.c_str() };
		Deserialize(parser);
	}

	void SceneSerializer::Deserialize(const std::string& filePath)
	{
		PROFILE_SCOPE(DeserializeScene, Serialization);

		JsonFileParser parser = { filePath.c_str() };
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