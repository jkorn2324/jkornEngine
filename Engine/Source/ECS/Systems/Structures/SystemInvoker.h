#pragma once

#include "SystemTypes.h"
#include <functional>

namespace Engine
{
	class SystemHandlerBase;
	class ISystemBase;

	/**
	 * The system invoker function.
	 */
	class SystemInvoker
	{

	public:
		SystemInvoker(SystemHandlerBase& handlerBase)
			: m_handlerBase(handlerBase) { }

	private:
		std::vector<ISystemBase*>& GetSystems();

	public:
		template<typename TSystem, typename... TArgs, typename Func>
		void InvokeSystem(const Func& callback, TArgs&& ...args)
		{
			static_assert(IsValidSystem<TSystem>::value, "System must be valid in order to invoke.");
			std::vector<ISystemBase*>& systems = GetSystems();
			// TODO: Sort the systems.
			for (size_t index = 0; index < systems.size(); ++index)
			{
				ISystemBase* systemBase = systems[index];
				TSystem* system = dynamic_cast<TSystem*>(systemBase);
				if (system != nullptr)
				{
					callback(*system, std::forward<TArgs>(args)...);
				}
			}
		}

	private:
		SystemHandlerBase& m_handlerBase;
	};
}