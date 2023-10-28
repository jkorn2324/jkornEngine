#pragma once

#include <filesystem>

namespace Engine
{
	class Scene;
	class Entity;
	class JsonFileReader;

	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		void Serialize(const std::filesystem::path& filePath);
		void Deserialize(const std::filesystem::path& filePath);

	private:
		void Deserialize(const JsonFileReader& jsonParser);

	private:
		Scene* m_scene;
	};
}