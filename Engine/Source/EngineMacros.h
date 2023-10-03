#pragma once

#include <type_traits>

namespace Engine
{

#define ENABLE_THREADING 0
	
// Default Macros.
#define NAMEOF(v) #v

// Binds functions to a std::function based on its name.
#define BIND_STATIC_FUNCTION(func) [](auto&&...args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }
#define BIND_CLASS_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }


// Windows Specific Macros
#if defined(PLATFORM_WINDOWS)

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


// The compile time type_trait that determines whether or not the type has a static function named func
#define DEFINE_TYPE_TRAIT_STATIC_FUNC(TraitName, func) template<typename T> \
	struct HasStaticFunc_##TraitName \
	{ \
	private: \
		template<typename U> \
		static auto TestFunc(int) -> decltype(&T::##func, std::true_type{}); \
		template<typename U> \
		static std::false_type TestFunc(...); \
	public: \
		static constexpr bool value = decltype(TestFunc<T>(0))::value; \
	}; \
	template<typename T> \
	using TraitName = HasStaticFunc_##TraitName<T>



// The compile time type_trait that determines whether or not the type has a member function named func
#define DEFINE_TYPE_TRAIT_MEMBER_FUNC(TraitName, func) template<typename T> \
	struct HasMemberFunc_##TraitName \
	{ \
	private: \
		template<typename U> \
		static auto TestFunc(int) -> decltype(std::declval<U>().##func(), std::true_type{}); \
		template<typename U> \
		static std::false_type TestFunc(...); \
	public: \
		static constexpr bool value = decltype(TestFunc<T>(0))::value; \
	}; \
	template<typename T> \
	using TraitName = HasMemberFunc_##TraitName<T>


// The compile time type_trait that determines whether or not the type has a member function named func	
#define DEFINE_TYPE_TRAIT_MEMBER_FUNC_ONE_PARAM(TraitName, func) template<typename T, typename TParameter> \
	struct HasMemberFunc_##TraitName \
	{ \
	private: \
		template<typename U> \
		static auto TestFunc(int) -> decltype(std::declval<U>.##func(std::declval<TParameter>()), std::true_type{}); \
		template<typename U> \
		static std::false_type TestFunc(...); \
	public: \
		static constexpr bool value = decltype(TestFunc<T>(0))::value; \
	}; \
	template<typename T, typename TParameter> \
	using TraitName = HasMemberFunc_##TraitName<T, TParameter>

#endif

// The MacOS Platform
#if defined(PLATFORM_MACOSX)

// Defines the serializable asset macro along with functions.
// Must forward declare the AssetSerializer & AssetCache classes as a template before using this macro.
#define SERIALIZABLE_ASSET(name) private: \
static bool DeserializeFromFile( name & asset, struct AssetDeserializationFileData& deserializeData); \
static bool SerializeToFile( name & asset, struct AssetSerializationFileData& serializeData); \
static bool DeserializeMetaFile( name & asset, struct AssetDeserializationMetaFileData& metaFileData); \
static bool SerializeToMetaFile( name & asset, struct AssetSerializationMetaFileData& metaFileData); \
static bool Create(std::shared_ptr< name >& outputAsset); \
    static bool Create( name ** outputAsset); \
friend class Engine::AssetSerializer< name >; \
friend class Engine::AssetCache< name >

// The compile time type_trait that determines whether or not the type has a static function named func
#define DEFINE_TYPE_TRAIT_STATIC_FUNC(TraitName, func) template<typename T> \
struct HasStaticFunc_##TraitName \
    { \
    private: \
        template<typename U> \
        static auto TestFunc(int) -> decltype(&T:: func, std::true_type{}); \
        template<typename U> \
        static std::false_type TestFunc(...); \
    public: \
        static constexpr bool value = decltype(TestFunc<T>(0))::value; \
    }; \
    template<typename T> \
    using TraitName = HasStaticFunc_##TraitName<T>



// The compile time type_trait that determines whether or not the type has a member function named func
#define DEFINE_TYPE_TRAIT_MEMBER_FUNC(TraitName, func) template<typename T> \
    struct HasMemberFunc_ TraitName \
    { \
    private: \
        template<typename U> \
        static auto TestFunc(int) -> decltype(std::declval<U>(). func(), std::true_type{}); \
        template<typename U> \
        static std::false_type TestFunc(...); \
    public: \
        static constexpr bool value = decltype(TestFunc<T>(0))::value; \
    }; \
    template<typename T> \
    using TraitName = HasMemberFunc_ TraitName<T>


// The compile time type_trait that determines whether or not the type has a member function named func
#define DEFINE_TYPE_TRAIT_MEMBER_FUNC_ONE_PARAM(TraitName, func) template<typename T, typename TParameter> \
    struct HasMemberFunc_ TraitName \
    { \
    private: \
        template<typename U> \
        static auto TestFunc(int) -> decltype(std::declval<U>. func(std::declval<TParameter>()), std::true_type{}); \
        template<typename U> \
        static std::false_type TestFunc(...); \
    public: \
        static constexpr bool value = decltype(TestFunc<T>(0))::value; \
    }; \
    template<typename T, typename TParameter> \
    using TraitName = HasMemberFunc_ TraitName<T, TParameter>

#endif

}
