#pragma once

#include "AssetCache.h"
#include "Shader.h"
#include "Texture.h"

namespace Engine
{

	class AssetManager
	{
	public:
		static AssetCache<Texture>& GetTextures();
		static AssetCache<Shader>& GetShaders();

	private:
		static AssetCache<Texture> s_textureAssets;
		static AssetCache<Shader> s_shaderAssets;
	};
}