#pragma once

namespace Engine
{
	class Scene;
	class Entity;
	class JsonFileParser;

	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		void Serialize(const std::wstring& filePath);
		void Serialize(const std::string& filePath);

		void Deserialize(const std::wstring& filePath);
		void Deserialize(const std::string& filePath);

	private:
		void Deserialize(const JsonFileParser& jsonParser);

	private:
		Scene* m_scene;
	};
}