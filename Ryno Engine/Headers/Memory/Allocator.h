#pragma once
#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "ReferenceAllocator.h"
#include <iostream>

#define LOG 0

namespace Ryno{


	enum MemoryLocation{ STACK_TEMP, STACK_PERS, REF_TEMP, REF_PERS };

	class Allocator{
	public:
		static void* alloc(size_t size, MemoryLocation mem_loc){
			void* ptr;
			switch (mem_loc){
			case STACK_TEMP:

				ptr = stack_allocator->temp_alloc(size);
#if LOG == 1 
				std::cout << "Stack Temp";
#endif
				break;
			case STACK_PERS:
				ptr = stack_allocator->pers_alloc(size);
#if LOG == 1 
				std::cout << "Stack Pers";
#endif
				break;
			case REF_TEMP: 
				ptr = reference_allocator->temp_alloc(size);
#if LOG == 1 
				std::cout << "Ref Temp";
#endif

				break;
			case REF_PERS: 
				ptr = reference_allocator->pers_alloc(size);
#if LOG == 1 
				std::cout << "Ref Pers";
#endif

				break;
			default: ptr = nullptr;
			}
#if LOG == 1 
			std::cout << ": allocated at " << ptr << std::endl;
#endif


			return ptr;
		}

		static StackAllocator* stack_allocator;
		static ReferenceAllocator* reference_allocator;
		static PoolAllocator* pool_allocator;
	};

}