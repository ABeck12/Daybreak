#pragma once

#include <memory>

#ifdef DB_PLATFORM_WINDOWS
#else
#error "Only Windows Supported"
#endif

#ifdef DB_PLATFORM_WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
// #elif DB_PLATFORM_LINUX | DB_PLATFORM_MAC
// #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef DB_ENABLE_ASSERTS
#define DB_MAKE_ASSERT_MESSAGE(msg, ...) fmt::format(msg, __VA_ARGS__)
#define DB_ASSERT(x, msg, ...)                                                                                                           \
	{                                                                                                                                    \
		if (!(x))                                                                                                                        \
		{                                                                                                                                \
			DB_ERROR("Assertion Failed in file {0} on line {1}: {2}", __FILENAME__, __LINE__, DB_MAKE_ASSERT_MESSAGE(msg, __VA_ARGS__)); \
			__debugbreak();                                                                                                              \
		}                                                                                                                                \
	}
#define DB_CORE_ASSERT(x, msg, ...)                                                                                                           \
	{                                                                                                                                         \
		if (!(x))                                                                                                                             \
		{                                                                                                                                     \
			DB_CORE_ERROR("Assertion Failed in file {0} on line {1}: {2}", __FILENAME__, __LINE__, DB_MAKE_ASSERT_MESSAGE(msg, __VA_ARGS__)); \
			__debugbreak();                                                                                                                   \
		}                                                                                                                                     \
	}
#else
#define DB_ASSERT(x, ...)
#define DB_CORE_ASSERT(x, ...)
#endif

#define DB_FORCE_INLINE __forceinline

#define DB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)

#define DB_STRUCT_NAME(T) std::string(typeid(T).name()).erase(0, 6)
#define DB_CLASS_NAME(T)  std::string(typeid(T).name()).erase(0, 5)

namespace Daybreak
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}