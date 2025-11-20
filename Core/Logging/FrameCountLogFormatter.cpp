#include "FrameCountLogFormatter.h"
#include "CoreFramework/Renderer.h"

void FrameCountLogFormatter::format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest)
{
    auto s = fmt::format("{}", Renderer::Get().GetFrameCount() % 1000);
    dest.append(s.data(), s.data() + s.size());
}

std::unique_ptr<spdlog::custom_flag_formatter> FrameCountLogFormatter::clone() const
{
    return std::make_unique<FrameCountLogFormatter>();
}
