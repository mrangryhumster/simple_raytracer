#pragma once

#include "nthreading_pool.hpp"
#include "nthreading_pool.inl"

namespace nova
{
	namespace threading
	{
		using thread_pool = thread_pool_t<void*,size_t>;
	}
}
