#include "SystemHandlerBase.h"

namespace Engine
{
	/**
	 * The base system manager.
	 */
	template<typename TSystem>
	class SystemManager
	{
	public:
		/**
		 * Gets the system handler base that controls this system.
		 */
		static SystemHandlerBase<TSystem>& Get()
		{
			static SystemHandlerBase<TSystem> s_systemManager;
			return s_systemManager;
		}

		SystemManager() = delete;
	};
}