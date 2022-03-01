#include "EnginePCH.h"
#include "AssetManager.h"

namespace Engine
{
	AssetCache<Shader> s_shaderAssets = AssetCache<Shader>();

	AssetCache<Shader>& AssetManager::GetShaders()
	{
		return s_shaderAssets;
	}

	AssetCache<Mesh> s_meshAssets = AssetCache<Mesh>();

	AssetCache<Mesh>& AssetManager::GetMeshes()
	{
		return s_meshAssets;
	}

	AssetCache<Texture> s_textureAssets = AssetCache<Texture>(false);

	AssetCache<Texture>& AssetManager::GetTextures()
	{
		return s_textureAssets;
	}

	AssetCache<Material> s_materialAssets = AssetCache<Material>();

	AssetCache<Material>& AssetManager::GetMaterials()
	{
		return s_materialAssets;
	}

	void AssetManager::UncacheAssets()
	{
		s_shaderAssets.Clear();
		s_textureAssets.Clear();
		s_meshAssets.Clear();
		s_materialAssets.Clear();
	}
}