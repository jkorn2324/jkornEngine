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

	void AssetManager::UncacheAssets()
	{
		s_shaderAssets.Clear();
		s_textureAssets.Clear();
	}
}