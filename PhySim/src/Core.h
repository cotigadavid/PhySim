#pragma once

#include <memory>

#ifdef PS_DEBUG
#define PS_ENABLE_ASSERTS
#endif

#ifdef PS_ENABLE_ASSERTS
#define PS_ASSERT(x, ...) { if(!(x)) { PS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define PS_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}