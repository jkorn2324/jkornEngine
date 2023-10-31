#pragma once

#include "Allocator.h"

#include <string>

namespace Engine
{
    namespace Internals::Assets
    {
        // TODO: Implementation
    }

    /**
     * @brief Determines whether or not the type is an asset type.
     * @tparam T The type.
     * 
     * In order to say that the value is an asset type than it must be used.
     */
    template<typename T>
    struct IsAsset
    {
        static constexpr bool Value = false;
    };

    /**
     * @brief Represents an implementation for an asset.
     */
    template<typename T>
    class AssetImpl
    {
        static_assert(IsAsset<T>::Value, "The asset implementation must be a valid asset.");

    public:
        // TODO: Implementation
    };
}
