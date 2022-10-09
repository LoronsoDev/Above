#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

namespace Above
{
	class ABOVE_API Log
	{
	public:
		Log();
		~Log();

		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger;  }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

///Defining macros so we can strip it from dist builds

//Core log macros
#define AB_CORE_TRACE(...)      ::Above::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AB_CORE_INFO(...)       ::Above::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AB_CORE_WARN(...)       ::Above::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AB_CORE_ERROR(...)      ::Above::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AB_CORE_FATAL(...)      ::Above::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define AB_TRACE(...)      ::Above::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AB_INFO(...)       ::Above::Log::GetClientLogger()->info(__VA_ARGS__)
#define AB_WARN(...)       ::Above::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AB_ERROR(...)      ::Above::Log::GetClientLogger()->error(__VA_ARGS__)
#define AB_FATAL(...)      ::Above::Log::GetClientLogger()->fatal(__VA_ARGS__)