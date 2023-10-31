#include "EnginePCH.h"
#include "Allocator.h"

#include "Memory.h"

namespace Engine
{
    namespace Internals::Allocation
    {
        template<typename TAllocator, typename T, typename... TArgs>
        T* Allocate(TAllocator& allocator, TArgs&&... args)
        {
            static_assert(false, "Invalid Allocation.");
            return nullptr;
        }
    }
}

