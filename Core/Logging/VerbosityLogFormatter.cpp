#include "VerbosityLogFormatter.h"
#include "CoreFramework/Renderer.h"

void VerbosityLogFormatter::format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest)
{
    dest.append(VerbosityString.data(), VerbosityString.data() + VerbosityString.size());
}

std::unique_ptr<spdlog::custom_flag_formatter> VerbosityLogFormatter::clone() const
{
    return std::make_unique<VerbosityLogFormatter>();
}
