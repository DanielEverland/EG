#include "CategoryLogFormatter.h"

void CategoryLogFormatter::format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest)
{
    dest.append(CurrentCategory.data(), CurrentCategory.data() + CurrentCategory.size());
}

std::unique_ptr<spdlog::custom_flag_formatter> CategoryLogFormatter::clone() const
{
    return std::make_unique<CategoryLogFormatter>();
}
