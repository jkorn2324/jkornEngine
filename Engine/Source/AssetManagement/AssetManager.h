#pragma once

#include "AssetCache.h"
#include "AssetReferenceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "GUID.h"

#include <filesystem>
#include <memory>

namespace Engine
{
	class AssetMapper;

	// An asset manager that gets defined for each type of the data.
	template<typename TAsset>
	class TypedAssetManager
	{
	public:
		TypedAssetManager()
			: m_assetCache(),
			m_assetReferenceManager()
		{
			m_assetReferenceManager.SetAssetRemovedAllReferencesCallback(
				BIND_CLASS_FUNCTION(UnLoad));
		}

		bool Get(const std::wstring& name, AssetRef<TAsset>& assetReference)
		{
			TAsset* asset = m_assetCache.Get(name);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(name, asset, &m_assetReferenceManager);
			return true;
		}

		bool Cache(AssetRef<TAsset>& assetReference, const std::wstring& name)
		{
			return Cache(name, assetReference);
		}

		// Caches the asset to the asset manager based on default parameters.
		bool Cache(const std::wstring& name, AssetRef<TAsset>& assetReference)
		{
			TAsset* asset = m_assetCache.Cache(name);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(name, asset, &m_assetReferenceManager);
			return true;
		}

		template<typename...Args>
		bool Cache(AssetRef<TAsset>& assetReference, const std::wstring& name, Args&&...args)
		{
			return Cache(name, assetReference, std::forward<Args>(args)...);
		}

		template<typename...Args>
		bool Cache(const std::wstring& name, AssetRef<TAsset>& assetReference, Args&&...args)
		{
			TAsset* asset = m_assetCache.Cache(name, std::forward<Args>(args)...);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(name, asset, &m_assetReferenceManager);
			return true;
		}

		bool Load(const std::wstring& filePath, AssetRef<TAsset>& assetReference)
		{
			return Load(assetReference, filePath);
		}

		bool Load(AssetRef<TAsset>& assetReference, const std::wstring& filePath)
		{
			TAsset* asset = m_assetCache.Load(filePath);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(filePath, asset, &m_assetReferenceManager);
			return true;
		}

		template<typename...Args>
		bool Load(const std::wstring& filePath, AssetRef<TAsset>& assetReference, Args&&...args)
		{
			return Load(assetReference, filePath, std::forward<Args>(args)...);
		}

		template<typename...Args>
		bool Load(AssetRef<TAsset>& assetReference, const std::wstring& fileName, Args&&...args)
		{
			TAsset* asset = m_assetCache.Load(fileName, std::forward<Args>(args)...);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(fileName, asset, &m_assetReferenceManager);
			return true;
		}

		void Clear()
		{
			m_assetCache.Clear();
			m_assetReferenceManager.Clear();
		}

	private:
		void UnLoad(const std::wstring& filePath)
		{
			m_assetCache.Unload(filePath);
		}

	private:
		AssetCache<TAsset> m_assetCache;
		AssetReferenceManager<TAsset> m_assetReferenceManager;
	};

	// The static main asset manager class.
	class AssetManager
	{
	public:
		static void Init(const std::filesystem::path& guidsDatabasePath);
		static void Release();

		static TypedAssetManager<Texture>& GetTextures();
		static TypedAssetManager<Shader>& GetShaders();
		static TypedAssetManager<Mesh>& GetMeshes();
		static TypedAssetManager<Material>& GetMaterials();

	private:
		static AssetMapper& GetAssetMapper();

		friend struct AssetSerializerFuncs;
	};
}