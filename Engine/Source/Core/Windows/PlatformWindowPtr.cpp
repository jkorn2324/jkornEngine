#include "EnginePCH.h"
#include "PlatformWindowPtr.h"

namespace Engine
{
	namespace Platform::Internals
	{
		template<typename TWindowPtr>
		void AllocWindowPtr(WindowPtr<TWindowPtr>& ptr, WindowData& windowData)
		{
			static_assert(false);
		}

		template<typename TWindowPtr>
		void OnUpdate(WindowPtr<TWindowPtr>& ptr, Window& window)
		{
			static_assert(false);
		}

		template<typename TWindowPtr>
		void OnShutdown(WindowPtr<TWindowPtr>& ptr, Window& window)
		{
			static_assert(false);
		}

		template<typename TWindowPtr>
		bool IsValid(const WindowPtr<TWindowPtr>& ptr) 
		{
			static_assert(false);
			return false; 
		}

		template<typename TWindowPtr>
		void SetMinSize(WindowPtr<TWindowPtr>& ptr, uint32_t min, uint32_t max)
		{
			static_assert(false);
		}

		template<typename TWindowPtr>
		void SetVSync(WindowPtr<TWindowPtr>& ptr, const bool vSync)
		{
			static_assert(false);
		}

#if defined(PLATFORM_WINDOWS)
		template<typename TWindowPtr>
		HWND GetHWND(const WindowPtr<TWindowPtr>& ptr)
		{
			static_assert(false);
			return nullptr;
		}
#endif
	}
}