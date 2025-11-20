// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <spdlog/pattern_formatter.h>

class VerbosityLogFormatter : public spdlog::custom_flag_formatter
{
public:
    static inline std::string VerbosityString;
    
    void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override;
    std::unique_ptr<custom_flag_formatter> clone() const override;
};
