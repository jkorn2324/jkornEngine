#include "EnginePCH.h"
#include "AssetManager.h"

namespace Engine
{
	AssetCache<Shader> AssetManager::s_shaderAssets = AssetCache<Shader>();

	AssetCache<Shader>& AssetManager::GetShaders()
	{
		return s_shaderAssets;
	}

	AssetCache<Texture> AssetManager::s_textureAssets = AssetCache<Texture>();

	AssetCache<Texture>& AssetManager::GetTextures()
	{
		return s_textureAssets;
	}

	AssetCache<Mesh> AssetManager::s_meshAssets = AssetCache<Mesh>();

	AssetCache<Mesh>& AssetManager::GetMeshes()
	{
		return s_meshAssets;
	}

	void AssetManager::UncacheAssets()
	{
		s_shaderAssets.Clear();
		s_textureAssets.Clear();
		s_meshAssets.Clear();
	}
}