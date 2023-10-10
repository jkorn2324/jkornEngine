#pragma once

#include "AssetCache.h"
#include "AssetReferenceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

#include <filesystem>
#include <memory>
#include <unordered_map>

namespace Engine
{
	class AssetMapper;
	class GUID;

	// Asset Manager funcs struct namespace.
	struct TypedAssetManagerFuncs
	{
	private:
		static bool GetGUID(const std::wstring& name, GUID& guid);
		static void SetPath(const std::filesystem::path& path, const GUID& guid);

		template<typename T>
		friend class TypedAssetManager;
	};

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
			GUID guid;
			if (!GetGUID(guid, name))
			{
				return false;
			}
			TAsset* asset = m_assetCache.Get(guid);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(asset, guid, name, &m_assetReferenceManager);
			return true;
		}

		bool Cache(AssetRef<TAsset>& assetReference, const std::wstring& name)
		{
			return Cache(name, assetReference);
		}

		// Caches the asset to the asset manager based on default parameters.
		bool Cache(const std::wstring& name, AssetRef<TAsset>& assetReference)
		{
			GUID guid;
			GetGUID(guid, name);
			TAsset* asset = m_assetCache.Cache(guid);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(asset, guid, name, &m_assetReferenceManager);
			const auto& found = m_namesToGUIDs.find(name);
			if (found == m_namesToGUIDs.end())
			{
				m_namesToGUIDs.emplace(name, guid);
			}
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
			GUID guid;
			GetGUID(guid, name);
			TAsset* asset = m_assetCache.Cache(guid, std::forward<Args>(args)...);
			if (asset == nullptr)
			{
				return false;
			}
			assetReference = AssetRef<TAsset>(asset, guid, name, &m_assetReferenceManager);
			const auto& found = m_namesToGUIDs.find(name);
			if (found == m_namesToGUIDs.end())
			{
				m_namesToGUIDs.emplace(name, guid);
			}
			return true;
		}

		bool Load(const std::filesystem::path& filePath, AssetRef<TAsset>& assetReference)
		{
			return Load(assetReference, filePath);
		}

		bool Load(AssetRef<TAsset>& assetReference, const std::filesystem::path& filePath)
		{
			GUID guid;
			bool guidExists = GetGUID(guid, filePath);

			TAsset* asset = m_assetCache.Load(guid, filePath);
			if (asset == nullptr)
			{
				return false;
			}

			if (!guidExists)
			{
				TypedAssetManagerFuncs::SetPath(filePath, guid);
			}
            
            const std::wstring wFilePath = filePath.wstring();
			assetReference = AssetRef<TAsset>(asset, guid, wFilePath, &m_assetReferenceManager);
			const auto& found = m_namesToGUIDs.find(wFilePath);
			if (found == m_namesToGUIDs.end())
			{
				m_namesToGUIDs.emplace(wFilePath, guid);
			}
			if (!GetGUID(guid, filePath))
			{
				TypedAssetManagerFuncs::SetPath(filePath, guid);
			}
			return true;
		}

		template<typename...Args>
		bool Load(const std::filesystem::path& filePath, AssetRef<TAsset>& assetReference, Args&&...args)
		{
			return Load(assetReference, filePath, std::forward<Args>(args)...);
		}

		template<typename...Args>
		bool Load(AssetRef<TAsset>& assetReference, const std::filesystem::path& fileName, Args&&...args)
		{
			GUID guid;
			bool guidExists = GetGUID(guid, fileName);
			TAsset* asset = m_assetCache.Load(guid, fileName, std::forward<Args>(args)...);
			if (asset == nullptr)
			{
				return false;
			}

			if (!guidExists)
			{
				TypedAssetManagerFuncs::SetPath(fileName, guid);
			}
            const std::wstring wFileName = fileName.wstring();
			assetReference = AssetRef<TAsset>(asset, guid, wFileName, &m_assetReferenceManager);
			
			const auto& found = m_namesToGUIDs.find(wFileName);
			if (found == m_namesToGUIDs.end())
			{
				m_namesToGUIDs.emplace(wFileName, guid);
			}
			return true;
		}

		void Clear()
		{
			m_assetCache.Clear();
			m_assetReferenceManager.Clear();
		}

	private:
		void UnLoad(const GUID& guid, const std::wstring& name)
		{
			const auto& found = m_namesToGUIDs.find(name);
			if (found != m_namesToGUIDs.end())
			{
				m_namesToGUIDs.erase(name);
			}
			m_assetCache.Unload(guid);
		}
        
        bool GetGUID(GUID& guid, const std::filesystem::path& path)
        {
            const std::wstring ws = path.wstring();
            return GetGUID(guid, ws);
        }

		bool GetGUID(GUID& guid, const std::wstring& name)
		{
			const auto& guidFound = m_namesToGUIDs.find(name);
			if (guidFound != m_namesToGUIDs.end())
			{
				guid = guidFound->second;
				return true;
			}
			TypedAssetManagerFuncs::GetGUID(name, guid);
			return false;
		}

	private:
		AssetCache<TAsset> m_assetCache;
		AssetReferenceManager<TAsset> m_assetReferenceManager;
		std::unordered_map<std::wstring, GUID> m_namesToGUIDs;
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

		static AssetMapper& GetAssetMapper();

	private:
		friend struct AssetSerializerFuncs;
	};
}
