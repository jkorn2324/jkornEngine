#pragma once

#include "EngineMacros.h"

namespace Engine
{

	/**
	 * The valid update system type.
	 */
	enum ValidSystemType
	{
		Type_UpdateSystem,
		Type_EventSystem
	};

	// Defines a type trait static function that determines if its a valid system.
	DEFINE_TYPE_TRAIT_STATIC_FUNC(IsValidSystem, GetSystemType);
}