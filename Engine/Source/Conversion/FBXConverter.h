#pragma once

namespace fbxsdk
{
	class FbxManager;
	class FbxScene;
}

namespace Engine
{

	class Mesh;
	class Scene;

	class FBXConverter
	{
	public:
		FBXConverter(const char* fileName);
		~FBXConverter();

		// TODO: Add more functions to read from an fbx.

		void ImportToScene(Scene& scene);

		bool IsValid() const { return m_fbxManager && m_fbxScene; }
		float GetScale() const { return m_scale; }

	private:
		bool Initialize();
		void LoadFBXFile(const char* fileName);

	private:
		fbxsdk::FbxManager* m_fbxManager;
		fbxsdk::FbxScene* m_fbxScene;

		float m_scale = 0.0f;
	};
}