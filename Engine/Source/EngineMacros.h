#pragma once

namespace Engine
{

#define ENABLE_THREADING 0

// Default Macros.
#define NAMEOF(v) #v

// Defines the serializable asset macro along with functions.
// Must forward declare the AssetSerializer & AssetCache classes as a template before using this macro.
#define SERIALIZABLE_ASSET(name) \
	private: \
	static bool DeserializeFromFile(##name& asset, struct AssetDeserializationFileData& deserializeData); \
	static bool SerializeToFile(##name& asset, struct AssetSerializationFileData& serializeData); \
	static bool DeserializeMetaFile(##name& asset, struct AssetDeserializationMetaFileData& metaFileData); \
	static bool SerializeToMetaFile(##name& asset, struct AssetSerializationMetaFileData& metaFileData); \
	static bool Create(std::shared_ptr<##name>& outputAsset); \
	static bool Create(##name** outputAsset); \
	friend class Engine::AssetSerializer<name>; \
	friend class Engine::AssetCache<name>

// Binds functions to a std::function based on its name.
#define BIND_STATIC_FUNCTION(func) [](auto&&...args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }
#define BIND_CLASS_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

}