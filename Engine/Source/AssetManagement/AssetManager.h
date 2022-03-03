#pragma once

#include "AssetCache.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "GUID.h"

#include <filesystem>

namespace Engine
{
	class AssetMapper;

	class AssetManager
	{
	public:
		static void Init(const std::filesystem::path& guidsDatabasePath);

		static AssetCache<Texture>& GetTextures();
		static AssetCache<Shader>& GetShaders();
		static AssetCache<Mesh>& GetMeshes();
		static AssetCache<Material>& GetMaterials();

		static AssetMapper& GetAssetMapper();

		static void Release();
	};
}