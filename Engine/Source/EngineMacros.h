#pragma once

namespace Engine
{

// Default Macros.
#define NAMEOF(v) #v

// Defines the serializable asset macro along with functions.
// Must forward declare the AssetSerializer & AssetCache classes as a template before using this macro.
#define SERIALIZABLE_ASSET(name) \
	private: \
    static void SerializeToMetaFile(##name& asset, struct AssetSerializationMetaData& prettyWriter); \
	static bool DeserializeFromMetaFile(##name& asset, struct AssetDeserializationMetaData& deserializeData); \
	static bool DeserializeFromFile(##name& asset, struct AssetDeserializationFileData& deserializeData); \
	static name* Create(); \
	friend class Engine::AssetSerializer<name>; \
	friend class Engine::AssetCache<name>

}