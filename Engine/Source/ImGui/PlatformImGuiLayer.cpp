#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

namespace Engine
{
    template<typename T, T Value>
    void Platform::Internals::OnLayerAddedImpl()
    {
        static_assert(false);
    }

    template<typename T, T Value>
    void Platform::Internals::OnLayerRemovedImpl()
    {
        static_assert(false);
    }

    template<typename T, T Value>
    void Platform::Internals::BeginFrameImpl()
    {
        static_assert(false);
    }

    template<typename T, T Value>
    void Platform::Internals::EndFrameImpl()
    {
        static_assert(false);
    }
}
