#pragma once

#include <vector>
#include <type_traits>
#include <algorithm>
#include <functional>

namespace Engine
{
	/**
	 * The system base.
	 */
	class ISystemBase;

	/**
	 * The System Handler Base.
	 */
	class SystemHandlerBase
	{
	private:
		const size_t c_ResizeAmount = 10;

	public:
		SystemHandlerBase()
			: m_systems() { }

		~SystemHandlerBase();

	private:
		/**
		 * Sets the system internally.
		 */
		void SetSystemInternal(ISystemBase* system)
		{
			// Pushes the systems back.
			m_systems.push_back(system);
		}

	public:
		/**
		 * Sets the system.
		 */
		template<typename TInheritedSystem, typename... TArgs>
		void SetSystem(TArgs&&... args)
		{
			static_assert(std::is_base_of<ISystemBase, TInheritedSystem>::value, "TInheritedSystem must inherit from TSystem, which inherits from ISystemBase");
			static_assert(std::is_constructible<TInheritedSystem, TArgs...>::value, "TInheritedSystem must be constructed based on the arguments inputted.");

			TInheritedSystem* system = new TInheritedSystem(std::forward<TArgs>(args)...);
			SetSystemInternal(static_cast<ISystemBase*>(system));
		}

		template<typename TInheritedSystem>
		void SetSystem()
		{
			static_assert(std::is_base_of<ISystemBase, TInheritedSystem>::value, "TInheritedSystem must inherit from TSystem, which inherits from ISystemBase");
			static_assert(std::is_default_constructible<TInheritedSystem>::value, "TInheritedSystem must be default constructable.");

			// Heap Alloc.
			// TODO: Make sure that the system is no longer heap alloc.
			TInheritedSystem* ptr = new TInheritedSystem();
			SetSystemInternal(static_cast<ISystemBase*>(ptr));
		}

		/**
		 * Gets the number of systems.
		 */
		const size_t GetNumSystems() const { return m_systems.size(); }

	private:
		std::vector<ISystemBase*> m_systems;

		friend class SystemInvoker;
	};
}