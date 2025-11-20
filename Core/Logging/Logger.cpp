#include "Logger.h"

#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>

#include "spdlog/sinks/basic_file_sink.h"
#include "FrameCountLogFormatter.h"
#include "VerbosityLogFormatter.h"
#include "Utilities/DirectoryHelpers.h"

using namespace spdlog;

void Logger::Init()
{
    auto formatter = std::make_unique<pattern_formatter>();
    formatter->add_flag<FrameCountLogFormatter>('*');
    formatter->add_flag<CategoryLogFormatter>('C');
    formatter->add_flag<VerbosityLogFormatter>('V');
    formatter->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%*][%C|%V]: %v");

    set_formatter(std::move(formatter));

    try 
    {
        std::string logFileName = DirectoryHelpers::GetLogsDirectory().string() + "EG.log";
        std::shared_ptr<logger> logger = basic_logger_mt("basic_logger", logFileName, true);
        set_default_logger(logger);
    }
    catch (const spdlog_ex &ex)
    {
        std::cerr << "Log init failed: " << ex.what() << std::endl;
        assert(false);
    }
}
