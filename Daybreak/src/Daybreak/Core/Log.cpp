#include "dbpch.h"

#include "Daybreak/Core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Daybreak
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		//spdlog::set_pattern("%^%T [%n] %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("DAYBREAK");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("SANDBOX");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}