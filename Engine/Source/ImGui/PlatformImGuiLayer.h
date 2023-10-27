#pragma once

#include <memory>
#include <type_traits>

#include "RenderingAPIType.h"
#include "WindowType.h"

namespace Engine
{

	enum LayerType
	{
		TYPE_NONE,
		TYPE_WINDOW,
		TYPE_GRAPHICS
	};

    namespace Platform::Internals
    {
        template<typename T, T Value>
        void OnLayerAddedImpl();
        template<typename T, T Value>
        void OnLayerRemovedImpl();
        template<typename T, T Value>
        void BeginFrameImpl();
        template<typename T, T Value>
        void EndFrameImpl();
    }

    template<typename T, T Value>
    struct PlatformImGuiLayerDefined
    {
        static constexpr bool IsDefined = false;
    };

    template<typename T, T Value>
    struct PlatformImGuiLayerInfo
    {
        static constexpr bool IsDefined = PlatformImGuiLayerDefined<T, Value>::IsDefined;
        static constexpr bool IsWindowLayer = std::is_same<T, WindowAPIType>();
        static constexpr bool IsGraphicsLayer = std::is_same<T, RenderingAPIType>();
    };

    template<typename T, T Value>
    class TPlatformImGuiLayer
    {
    public:
        // The Layer Information Associated with the ImGuiLayer.
        static constexpr PlatformImGuiLayerInfo<T, Value> LayerInfo;
        
        // Constructor.
        TPlatformImGuiLayer() = default;
        
        void OnLayerAdded() { Platform::Internals::OnLayerAddedImpl<T, Value>(); }
        void OnLayerRemoved() { Platform::Internals::OnLayerRemovedImpl<T, Value>(); }
        
        void BeginFrame() { Platform::Internals::BeginFrameImpl<T, Value>(); }
        void EndFrame() { Platform::Internals::EndFrameImpl<T, Value>(); }
    };

    // Defines the Platform Graphics ImGui Layer.
    using PlatformGraphicsImGuiLayer = TPlatformImGuiLayer<RenderingAPIType, g_ActiveRenderingAPIType>;
    using PlatformWindowImGuiLayer = TPlatformImGuiLayer<WindowAPIType, g_ActiveWindowAPIType>;
}
