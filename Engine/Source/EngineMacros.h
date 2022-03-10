#pragma once

namespace Engine
{

// Default Macros.
#define NAMEOF(v) #v

// Defines the serializable asset macro along with functions.
// Must forward declare the AssetSerializer & AssetCache classes as a template before using this macro.
#define SERIALIZABLE_ASSET(name) \
	public: \
	const GUID& GetGUID() const; \
	protected: \
	GUID m_guid; \
	private: \
	static bool DeserializeFromFile(##name& asset, struct AssetDeserializationFileData& deserializeData); \
	static name* Create(); \
	friend class Engine::AssetSerializer<name>; \
	friend class Engine::AssetCache<name>

// Binds functions to a std::function based on its name.
#define BIND_STATIC_FUNCTION(func) [](auto&&...args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }
#define BIND_CLASS_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

}