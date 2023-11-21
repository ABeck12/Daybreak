#pragma once

#include <memory>

#ifdef DB_PLATFORM_WINDOWS
#else
//#error "Only Windows Supported"
#endif

#ifdef DB_ENABLE_ASSERTS
#define DB_ASSERT(x, ...) { if(!(x)) { DB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define DB_CORE_ASSERT(x, ...) { if(!(x)) { DB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define DB_ASSERT(x, ...)
#define DB_CORE_ASSERT(x, ...)
#endif

#define DB_FORCE_INLINE __forceinline

#define DB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)

namespace Daybreak
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}