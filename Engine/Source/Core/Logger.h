#pragma once

#include <memory>
#include <spdlog\spdlog.h>

namespace Engine
{

	class Logger
	{
	public:
		static bool Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetExternalLogger();
	};

#define DEBUG_LOG_CORE(...) Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define ERROR_LOG_CORE(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
#define INFO_LOG_CORE(...) Logger::GetCoreLogger()->info(__VA_ARGS__)
#define WARN_LOG_CORE(...) Logger::GetCoreLogger()->warn(__VA_ARGS__)

#define DEBUG_LOG_EXTERNAL(...) Engine::Logger::GetExternalLogger()->debug(__VA_ARGS__)
#define ERROR_LOG_EXTERNAL(...) Engine::Logger::GetExternalLogger()->error(__VA_ARGS__)
#define INFO_LOG_EXTERNAL(...) Engine::Logger::GetExternalLogger()->info(__VA_ARGS__)
#define WARN_LOG_EXTERNAL(...) Engine::Logger::GetExternalLogger()->warn(__VA_ARGS__)

}
