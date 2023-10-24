#pragma once

namespace Engine
{
	// Forward Declarations.
	template<typename T>
	struct WindowPtr;
	class Window;
	struct WindowData;

	namespace Platform::Internals
	{
		template<typename TWindowPtr>
		void AllocWindowPtr(WindowPtr<TWindowPtr>& ptr, WindowData& windowData);

		template<typename TWindowPtr>
		void OnUpdate(WindowPtr<TWindowPtr>& ptr, Window& window);

		template<typename TWindowPtr>
		void OnShutdown(WindowPtr<TWindowPtr>& ptr, Window& window);

		template<typename TWindowPtr>
		bool IsValid(const WindowPtr<TWindowPtr>& ptr);

		template<typename TWindowPtr>
		void SetMinSize(WindowPtr<TWindowPtr>& ptr, uint32_t min, uint32_t max);

		template<typename TWindowPtr>
		void SetVSync(WindowPtr<TWindowPtr>& ptr, const bool vSync);
    
        template<typename TWindowPtr>
        void* GetDeviceWindowPtr(const WindowPtr<TWindowPtr>& ptr);
	}

	template<typename T>
	struct WindowPtr
	{
		/**
		 * Determines whether or not the platform window pointer is valid. 
		 */
		static constexpr bool IsValidPtr = false;

		/**
		 * The window pointer. 
		 */
		WindowPtr(WindowData& windowData) 
			: m_ptr(nullptr)
		{
			Platform::Internals::AllocWindowPtr(*this, windowData);
		}

		/**
		 * Called to update the window. 
		 */
		void OnUpdate(Window& window) 
		{ 
			Platform::Internals::OnUpdate(*this, window);
		}

		/**
		 * Called when the window gets shut down.
		 */
		void OnShutdown(Window& window) 
		{ 
			Platform::Internals::OnShutdown(*this, window);
		}

		/**
		 * Determines whether or not the platform window is valid. 
		 */
		bool IsValid() const { return Platform::Internals::IsValid(*this); }

		/**
		 * Sets the min size.
		 */
		void SetMinSize(uint32_t width, uint32_t height) 
		{
			Platform::Internals::SetMinSize(*this, width, height);
		}

		/**
		 * Sets whether or not we are doing vsync. 
		 */
		void SetVSync(const bool vSync) 
		{
			Platform::Internals::SetVSync(*this, vSync);
		}
 
        void* GetDeviceWindowPtr() const { return Platform::Internals::GetDeviceWindowPtr<T>(*this); }

	public:
		T* m_ptr;
	};
}
