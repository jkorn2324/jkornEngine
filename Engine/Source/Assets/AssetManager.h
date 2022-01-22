#pragma once

#include "AssetCache.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

namespace Engine
{

	class AssetManager
	{
	public:
		static AssetCache<Texture>& GetTextures();
		static AssetCache<Shader>& GetShaders();
		static AssetCache<Mesh>& GetMeshes();
		static AssetCache<Material>& GetMaterials();

		static void UncacheAssets();
	};
}