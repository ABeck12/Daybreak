#pragma once

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

#define BIT(x) (1 << x)