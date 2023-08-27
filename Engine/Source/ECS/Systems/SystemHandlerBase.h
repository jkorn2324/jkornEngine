#pragma once

#include <vector>
#include <type_traits>
#include <algorithm>
#include <functional>

namespace Engine
{

	/**
	 * The update system.
	 */
	class ISystemBase;

	/**
	 * The System Function.
	 */
	template<typename TContext, typename TInheritedSystem>
	using SystemFunc = std::function<void(const TContext&, TInheritedSystem*)>;

	/**
	 * The System Handler Base.
	 */
	template<typename TSystem>
	class SystemHandlerBase
	{
		static_assert(std::is_base_of<ISystemBase, TSystem>::value, "TSystem Must be inherited from the ISystemBase.");

	private:
		/**
		 * The System Reference (Contains the id)
		 */
		struct SystemRef
		{
			TSystem* system;
			uint32_t orderId;

			friend bool operator<(const SystemRef& a, const SystemRef& b)
			{
				return a.orderId < b.orderId;
			}

			friend bool operator<=(const SystemRef& a, const SystemRef& b)
			{
				return a.orderId <= b.orderId;
			}

			friend bool operator>(const SystemRef& a, const SystemRef& b)
			{
				return a.orderId > b.orderId;
			}

			friend bool operator>=(const SystemRef& a, const SystemRef& b)
			{
				return a.orderId >= b.orderId;
			}
		};


	private:
		const size_t c_ResizeAmount = 10;

	public:
		SystemHandlerBase()
			: m_systems() { }

		~SystemHandlerBase()
		{
			size_t systemSize = m_systems.size();
			for (size_t index = 0; index < systemSize; ++index)
			{
				SystemRef& system = m_systems[index];
				// Deallocates the systems.
				delete system.system;
			}
			m_systems.empty();
		}

	private:
		/**
		 * Sets the system internally.
		 */
		void SetSystemInternal(uint32_t orderId, TSystem* system)
		{
			SystemRef newSystem = { system, orderId };
			m_systems.push_back(newSystem);
			std::sort(m_systems.begin(), m_systems.end(), std::less<SystemRef>());
		}

	public:
		/**
		 * Sets the system.
		 */
		template<typename TInheritedSystem, typename... TArgs>
		void SetSystem(uint32_t orderId, TArgs&&... args)
		{
			static_assert(std::is_base_of<TSystem, TInheritedSystem>::value, "TInheritedSystem must inherit from TSystem, which inherits from ISystemBase");
			static_assert(std::is_constructible<TInheritedSystem, TArgs...>::value, "TInheritedSystem must be constructed based on the arguments inputted.");

			TInheritedSystem* system = new TInheritedSystem(std::forward<TArgs>(args)...);
			SetSystemInternal(orderId, static_cast<TSystem*>(system));
		}

		template<typename TInheritedSystem>
		void SetSystem(uint32_t orderId)
		{
			static_assert(std::is_base_of<TSystem, TInheritedSystem>::value, "TInheritedSystem must inherit from TSystem, which inherits from ISystemBase");
			static_assert(std::is_default_constructible<TInheritedSystem>::value, "TInheritedSystem must be default constructable.");

			// Heap Alloc.
			// TODO: Make sure that the system is no longer heap alloc.
			TInheritedSystem* ptr = new TInheritedSystem();
			SetSystem(orderId, ptr);
		}

		/**
		 * Invokes the systems from the system manager.
		 */
		template<typename TContext, typename TInheritedSystem>
		void InvokeSystem(const TContext& ctx, const SystemFunc<TContext, TInheritedSystem>& callback)
		{
			static_assert(std::is_base_of<TSystem, TInheritedSystem>::value, "TInheritedSystem must inherit from TSystem, which inherits from ISystemBase");

			if (!callback)
			{
				return;
			}

			for (size_t i = 0; i < m_systems.size(); ++i)
			{
				const SystemRef& systemRef = m_systems[i];
				TInheritedSystem* castedSys = dynamic_cast<TInheritedSystem*>(systemRef.system);
				if (castedSys != nullptr && callback)
				{
					callback(ctx, castedSys);
				}
			}
		}

	private:
		std::vector<SystemRef> m_systems;
	};
}