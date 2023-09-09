#pragma once

#include "SystemHandlerBase.h"
#include "SystemInvoker.h"

namespace Engine
{
	class SystemHandlerBase;

	/**
	 * The base system manager.
	 */
	class SystemManager
	{
	private:
		/**
		 * Gets the list of all systems.
		 */
		static SystemHandlerBase& Get();

		// TODO: Need to figure out a way to serialize the systems and then load them in at runtime.

	public:
		template<typename TSystem, typename... TArgs, typename TFunc>
		static void Invoke(const TFunc& func, TArgs&&...args)
		{
			SystemInvoker invoker(Get());
			invoker.InvokeSystem<TSystem>(func, std::forward<TArgs>(args)...);
		}

		template<typename TSystem>
		static void AddSystem()
		{
			SystemHandlerBase& system = Get();
			system.AddSystem<TSystem>();
		}

		template<typename TSystem, typename...TArgs>
		static void AddSystem(TArgs&&... args)
		{
			SystemHandlerBase& system = Get();
			system.AddSystem<TSystem, TArgs...>(std::forward<TArgs>(args)...);
		}
	};
}