#pragma once


namespace Engine
{

	class SceneManager
	{
	public:
		static void Init();
		static void Release();

		static void LoadScene(const wchar_t* filePath);

		static class Scene& GetActiveScene();

		static void OnRuntimeUpdate(const class Timestep& ts);
		static void OnEditorUpdate(const class Timestep& ts);
	};
}