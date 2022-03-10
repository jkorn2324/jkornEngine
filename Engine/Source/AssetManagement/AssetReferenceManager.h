#pragma once

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
			m_asset(nullptr),
			m_assetName(L"")
		{

		}

		AssetRef(const std::wstring& assetName, TAsset* asset, AssetReferenceManager<TAsset>* referenceManager)
			: m_asset(asset),
			m_referenceManager(referenceManager),
			m_assetName(assetName)
		{
			m_referenceManager->AddReference(asset);
		}
		
		~AssetRef()
		{
			DeReference();
		}

		AssetRef(const AssetRef& assetRef)
			: m_asset(assetRef.m_asset),
			m_referenceManager(assetRef.m_referenceManager),
			m_assetName(assetRef.m_assetName)
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
			m_assetName = assetRef.m_assetName;
			if (m_referenceManager != nullptr)
			{
				m_referenceManager->AddReference(m_asset);
			}
			return *this;
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
				m_referenceManager->RemoveReference(m_asset, m_assetName);
			}
			m_asset = nullptr;
		}

	private:
		TAsset* m_asset;
		AssetReferenceManager<TAsset>* m_referenceManager;
		std::wstring m_assetName;
	};

	// Reference counted structure.
	struct RefCount
	{
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
		void AddReference(TAsset* asset)
		{
			if (asset == nullptr) return;
			const auto& found = m_referenceCountedAssets.find(asset);
			if (found != m_referenceCountedAssets.end())
			{
				found->second.referenceCount++;
				return;
			}
			m_referenceCountedAssets.emplace(asset, RefCount{ 1 });
		}

		void RemoveReference(TAsset* asset, const std::wstring& assetName)
		{
			if (asset == nullptr) return;
			const auto& found = m_referenceCountedAssets.find(asset);
			if (found != m_referenceCountedAssets.end())
			{
				found->second.referenceCount--;
				if (found->second.referenceCount <= 0)
				{
					m_referenceCountedAssets.erase(found);
					if (m_assetRemovedAllReferencesEvent != nullptr)
					{
						m_assetRemovedAllReferencesEvent(assetName);
					}
				}
			}
		}

		void Clear()
		{
			m_referenceCountedAssets.clear();
		}

		void SetAssetRemovedAllReferencesCallback(const std::function<void(const std::wstring&)>& event)
		{
			m_assetRemovedAllReferencesEvent = event;
		}

	private:
		std::unordered_map<TAsset*, RefCount> m_referenceCountedAssets;
		std::function<void(const std::wstring&)> m_assetRemovedAllReferencesEvent = nullptr;
	};
}
