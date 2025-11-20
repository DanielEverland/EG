// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <filesystem>

class TestDirectoryHelper
{
public:
    static std::filesystem::path RelativeToFullPath(const std::filesystem::path& path);
    static std::filesystem::path GetContentPath(const std::filesystem::path& path);
};
