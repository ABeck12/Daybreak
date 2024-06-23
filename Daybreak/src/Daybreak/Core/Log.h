#pragma once

#include "dbpch.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Daybreak
{

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#if DB_DEBUG || DB_RELEASE

#define DB_CORE_LOG(...)    ::Daybreak::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DB_CORE_INFO(...)   ::Daybreak::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DB_CORE_WARN(...)   ::Daybreak::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DB_CORE_ERROR(...)  ::Daybreak::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DB_CORE_FATAL(...)  ::Daybreak::Log::GetCoreLogger()->critical(__VA_ARGS__)
						   
#define DB_LOG(...)         ::Daybreak::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DB_INFO(...)        ::Daybreak::Log::GetClientLogger()->info(__VA_ARGS__)
#define DB_WARN(...)        ::Daybreak::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DB_ERROR(...)       ::Daybreak::Log::GetClientLogger()->error(__VA_ARGS__)
#define DB_FATAL(...)       ::Daybreak::Log::GetClientLogger()->critical(__VA_ARGS__)

#elif DB_DIST

#define DB_CORE_LOG(...)   
#define DB_CORE_INFO(...)  
#define DB_CORE_WARN(...)  
#define DB_CORE_ERROR(...) 
#define DB_CORE_FATAL(...) 

#define DB_LOG(...)        
#define DB_INFO(...)       
#define DB_WARN(...)       
#define DB_ERROR(...)      
#define DB_FATAL(...) 

#endif