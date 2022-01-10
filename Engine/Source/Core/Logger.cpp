#include "EnginePCH.h"
#include "Logger.h"

#include <spdlog\sinks\stdout_color_sinks.h>
#include <spdlog\sinks\basic_file_sink.h>

namespace Engine
{

	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_externalLogger;

	static void InitializeLogger(const char* name,
		std::shared_ptr<spdlog::logger>& logger,
		const std::vector<spdlog::sink_ptr>& loggerSinks)
	{
		logger = std::make_shared<spdlog::logger>(name, loggerSinks.begin(), loggerSinks.end());
		spdlog::register_logger(logger);

		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
	}

	bool Logger::Init()
	{
		std::vector<spdlog::sink_ptr> loggerSinks;
		loggerSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		loggerSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
			"Engine.log", true));

		loggerSinks[0]->set_pattern("%^%X %n [%l]: %v%$");
		loggerSinks[1]->set_pattern("[%X] %n [%l]: %v");

		InitializeLogger("Engine", s_coreLogger, loggerSinks);
		InitializeLogger("External", s_externalLogger, loggerSinks);
		return false;
	}
	
	std::shared_ptr<spdlog::logger>& Logger::GetCoreLogger()
	{
		DebugAssert(s_coreLogger != nullptr,
			"Core Logger doesn't exist yet.");
		return s_coreLogger;
	}

	std::shared_ptr<spdlog::logger>& Logger::GetExternalLogger()
	{
		DebugAssert(s_externalLogger != nullptr,
			"External Logger doesn't exist yet.");
		return s_externalLogger;
	}
}