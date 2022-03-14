#pragma once

#include "GUID.h"

#include <cstdint>
#include <unordered_map>
#include <string>
#include <functional>

namespace Engine
{
	template<typename T>
	class AssetReferenceManager;

	// Reference counts the asset based on an asset reference manager.
	template<typename TAsset>
	class AssetRef
	{

	public:
		AssetRef()
			: m_referenceManager(nullptr),
			m_asset(nullptr)
		{

		}

		AssetRef(TAsset* asset, const GUID& guid, const std::wstring& name, AssetReferenceManager<TAsset>* referenceManager)
			: m_asset(asset),
			m_referenceManager(referenceManager)
		{
			m_referenceManager->AddReference(asset, name, guid);
		}
		
		~AssetRef()
		{
			DeReference();
		}

		AssetRef(const AssetRef& assetRef)
			: m_asset(assetRef.m_asset),
			m_referenceManager(assetRef.m_referenceManager)
		{
			m_referenceManager->AddReference(m_asset);
		}

		AssetRef& operator=(const AssetRef& assetRef)
		{
			if (m_asset != nullptr)
			{
				DeReference();
			}
			m_referenceManager = assetRef.m_referenceManager;
			m_asset = assetRef.m_asset;
			if (m_referenceManager != nullptr)
			{
				m_referenceManager->AddReference(m_asset);
			}
			return *this;
		}
		
		bool GetGUID(GUID& guid) const
		{
			if (m_referenceManager == nullptr)
			{
				return false;
			}
			return m_referenceManager->GetGUID(m_asset, guid);
		}

		TAsset* Get() const { return m_asset; }

		// Dereference for a ptr operator overload.
		TAsset* operator->() const noexcept
		{
			return m_asset;
		}

		// Dereference operator overload.
		TAsset& operator*() const noexcept
		{
			return *m_asset;
		}

		operator bool() const
		{
			return m_asset != nullptr;
		}

		operator bool()
		{
			return m_asset != nullptr;
		}

		friend bool operator==(const AssetRef<TAsset>& a, const AssetRef<TAsset>& b)
		{
			return a.Get() == b.Get();
		}

		friend bool operator!=(const AssetRef<TAsset>& a, const AssetRef<TAsset>& b)
		{
			return a.Get() != b.Get();
		}

	private:
		void DeReference()
		{
			if (m_referenceManager != nullptr)
			{
				m_referenceManager->RemoveReference(m_asset);
			}
			m_asset = nullptr;
		}

	private:
		TAsset* m_asset;
		AssetReferenceManager<TAsset>* m_referenceManager;
	};

	// Reference counted structure.
	struct RefCount
	{
		std::wstring name;
		GUID guid;
		uint32_t referenceCount;
	};

	// TODO: Add mutexes.
	// The asset reference manager, manages whether to load and unload
	// assets based on how many references there are within the scene.
	template<typename TAsset>
	class AssetReferenceManager
	{
	public:
		AssetReferenceManager()
			: m_referenceCountedAssets() { }
		AssetReferenceManager(uint32_t capacity)
			: m_referenceCountedAssets(capacity) { }

	public:
		void AddReference(TAsset* asset, const std::wstring& name, const GUID& guid)
		{
			if (asset == nullptr) return;
			const auto& found = m_referenceCountedAssets.find(asset);
			if (found != m_referenceCountedAssets.end())
			{
				found->second.referenceCount++;
				return;
			}
			m_referenceCountedAssets.emplace(asset, RefCount{ name, guid, 1 });
		}

		void AddReference(TAsset* asset)
		{
			if (asset == nullptr) return;
			const auto& found = m_referenceCountedAssets.find(asset);
			if (found != m_referenceCountedAssets.end())
			{
				found->second.referenceCount++;
			}
		}

		bool GetGUID(TAsset* asset, GUID& guid) const
		{
			if (asset == nullptr) return false;
			const auto& found = m_referenceCountedAssets.find(asset);
			if (found != m_referenceCountedAssets.end())
			{
				guid = m_referenceCountedAssets->second.guid;
				return true;
			}
			return false;
		}

		void RemoveReference(TAsset* asset)
		{
			if (asset == nullptr) return;
			const auto& found = m_referenceCountedAssets.find(asset);
			if (found != m_referenceCountedAssets.end())
			{
				found->second.referenceCount--;
				if (found->second.referenceCount <= 0)
				{ 
					std::wstring name = found->second.name;
					GUID guid = found->second.guid;

					m_referenceCountedAssets.erase(found);
					if (m_assetRemovedAllReferencesEvent != nullptr)
					{
						m_assetRemovedAllReferencesEvent(guid, name);
					}
				}
			}
		}

		void Clear()
		{
			m_referenceCountedAssets.clear();
		}

		void SetAssetRemovedAllReferencesCallback(const std::function<void(const GUID&, const std::wstring&)>& event)
		{
			m_assetRemovedAllReferencesEvent = event;
		}

	private:
		std::unordered_map<TAsset*, RefCount> m_referenceCountedAssets;
		std::function<void(const GUID&, const std::wstring&)> m_assetRemovedAllReferencesEvent = nullptr;
	};
}
