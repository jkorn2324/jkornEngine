#pragma once

#include "EnttUtility.h"
#include <type_traits>

namespace Engine
{
	class Entity;

	// Determines whether or not its a valid component.
	template<typename TComponent>
	struct IsValidComponent
	{
		// The value for the copy component.
		static constexpr bool value = std::is_copy_constructible<TComponent>::value && std::is_copy_assignable<TComponent>::value;
	};
}