// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <spdlog/spdlog.h>

#include "CategoryLogFormatter.h"
#include "VerbosityLogFormatter.h"

enum Category : uint8_t
{
    Engine,
    Game
};
namespace CategoryUtil
{
    inline std::string ToString(Category v)
    {
        switch (v)
        {
        case Engine: return "Engine";
        case Game: return "Game";
        default: return "N/A";
        }
    }
}

enum Verbosity : uint8_t
{
    Verbose,
    Info,
    Warning,
    Error
};
namespace VerbosityUtil
{
    inline std::string ToString(Verbosity v)
    {
        switch (v)
        {
            case Verbose: return "Verbose";
            case Info: return "Info";
            case Warning: return "Warning";
            case Error: return "Error";
            default: return "N/A";
        }
    }
}

class Logger
{
public:
    static void Init();

    template <typename... Args>
    static void Log(Category category, Verbosity verbosity, spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        CategoryLogFormatter::CurrentCategory = CategoryUtil::ToString(category);
        VerbosityLogFormatter::VerbosityString = VerbosityUtil::ToString(verbosity);
        
        switch (verbosity)
        {
        case Verbose:
        case Info:
            spdlog::info(fmt, std::forward<Args>(args)...);
            break;
        case Warning:
            spdlog::warn(fmt, std::forward<Args>(args)...);
            break;
        case Error:
            spdlog::error(fmt, std::forward<Args>(args)...);
        }
    }
};
