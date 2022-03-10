#pragma once

#include <filesystem>

namespace Engine
{
	class Scene;
	class Entity;
	class JsonFileParser;

	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		void Serialize(const std::filesystem::path& filePath);
		void Deserialize(const std::filesystem::path& filePath);

	private:
		void Deserialize(const JsonFileParser& jsonParser);

	private:
		Scene* m_scene;
	};
}