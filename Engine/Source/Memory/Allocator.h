#pragma once

#include "Memory.h"

namespace Engine
{
    namespace Internals::Allocation
    {
        /**
         * Determines whether or not there is an override allocator.
         */
        template<typename T, typename TAllocator>
        struct HasOverrideAllocator
        {
            static constexpr bool Value = false;
        };
    
        template<typename TAllocator, typename T, typename... TArgs>
        T* Allocate(TAllocator& allocator, TArgs&&... args);
    }

	template<typename TAllocator>
	struct IsAllocator
	{
		static constexpr bool Value = false;
	};
	/**
	 * The default allocator class (uses new & delete functionality)
	 */
	class DefaultAllocator
	{
	public:

		/**
		 * Allocate an instance. 
		 */
		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... args)
		{
            if constexpr (Internals::Allocation::HasOverrideAllocator<T, DefaultAllocator>::Value)
            {
                return Internals::Allocation::Allocate<DefaultAllocator, T, TArgs...>(*this, std::forward<TArgs>(args)...);
            }
			return Memory::Alloc<T>(std::forward<TArgs>(args)...);
		}

		/**
		 * Allocate an array. 
		 */
		template<typename T>
		T* Allocate(const size_t length)
		{
			return Memory::AllocArray<T>(length);
		}

		/**
		 * Deallocate a pointer instance.
		 */
		template<typename T>
		void DeAllocate(T*& ptr)
		{
			Memory::DeAlloc<T>(ptr);
		}

		/**
		 * Deallocate an array. 
		 */
		template<typename T>
		void DeAllocate(T*& ptr, const size_t length)
		{
			Memory::DeAllocArray<T>(ptr);
		}
	};

	template<>
	struct IsAllocator<DefaultAllocator>
	{
		static constexpr bool Value = true;
	};

	/**
	 * A type of allocator that uses the malloc & free functionality in c. 
	 */
	class HeapAllocator
	{
	public:

		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... args)
		{
			static_assert(std::is_constructible<T, ...TArgs>::value, "Object must be constructable using those args.");
            
            if constexpr (Internals::Allocation::HasOverrideAllocator<T, HeapAllocator>::Value)
            {
                return Internals::Allocation::Allocate<HeapAllocator, T, TArgs...>(*this, std::forward<TArgs>(args)...);
            }
			void* mallocMemory = (void*)Memory::Malloc(sizeof(T));
			return Memory::Construct(mallocMemory, std::forward<TArgs>(args)...);
		}

		template<typename T>
		T* Allocate(const size_t length)
		{
			return (T*)Memory::Malloc(sizeof(T) * length);
		}

		template<typename T>
		void DeAllocate(T*& ptr)
		{
			// Call the destructor explicitly.
			Memory::Destruct(ptr);
			Memory::Free((void*)ptr);
			ptr = nullptr;
		}

		template<typename T>
		void DeAllocate(T*& ptr, const size_t length)
		{
			Memory::Free((void*)ptr);
			ptr = nullptr;
		}
	};

	template<>
	struct IsAllocator<HeapAllocator>
	{
		static constexpr bool Value = true;
	};

}
