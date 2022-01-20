#pragma once

#include "AssetCache.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

namespace Engine
{

	class AssetManager
	{
	public:
		static AssetCache<Texture>& GetTextures();
		static AssetCache<Shader>& GetShaders();

		static void UncacheAssets();

	private:
		static AssetCache<Texture> s_textureAssets;
		static AssetCache<Shader> s_shaderAssets;
	};
}