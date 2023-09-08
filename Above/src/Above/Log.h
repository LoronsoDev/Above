#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Above
{
	class ABOVE_API Log
	{
	public:
		Log();
		~Log();

		static void Init();
		inline static Ref<spdlog::logger> GetCoreLogger() { return s_CoreLogger;  }
		inline static Ref<spdlog::logger> GetClientLogger() { return s_ClientLogger;  }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

///Defining macros so we can strip it from dist builds

//Core log macros
//TODO: Core macros can be run from the client application, triggering a CORE message that can be confusing.
#define AB_CORE_TRACE(...)      ::Above::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AB_CORE_INFO(...)       ::Above::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AB_CORE_WARN(...)       ::Above::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AB_CORE_ERROR(...)      ::Above::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AB_CORE_FATAL(...)      ::Above::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define AB_TRACE(...)			::Above::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AB_INFO(...)			::Above::Log::GetClientLogger()->info(__VA_ARGS__)
#define AB_WARN(...)			::Above::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AB_ERROR(...)			::Above::Log::GetClientLogger()->error(__VA_ARGS__)
#define AB_FATAL(...)			::Above::Log::GetClientLogger()->critical(__VA_ARGS__)