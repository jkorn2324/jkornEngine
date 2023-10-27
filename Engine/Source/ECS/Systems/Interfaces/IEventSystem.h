#pragma once

#include "EngineMacros.h"
#include "SystemTypes.h"
#include "ISystemBase.h"

namespace Engine
{
	// Defines a type trait w/ one parameter.
	DEFINE_TYPE_TRAIT_MEMBER_FUNC_ONE_PARAM(HasOnEventTriggeredFunc, OnEventTriggered);

	/**
	 * Interface system that gets invoked during the update.
	 */
	template<typename TEventContext>
	class IEventSystem : public ISystemBase
	{
	public:
		/**
		 * Called when the event system was triggered.
		 */
		void InvokeEventTriggered(const TEventContext& context)
		{
			OnEventTriggered(context);
		}

	protected:
		virtual void OnEventTriggered(const TEventContext& context) = 0;

	public:
		// Ensures that this is a valid system type.
		static constexpr ValidSystemType GetSystemType() { return Type_EventSystem; }
	};
}