// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <charconv>
#include <optional>
#include <string>
#include <type_traits>

class ConversionUtil
{
public:
    template<class T>
    requires std::is_integral_v<T>
    static std::optional<T> TryToInteger(const std::string& str)
    {
        T value = { };
        auto* begin = str.data();
        auto* end   = str.data() + str.size();

        auto [ptr, ec] = std::from_chars(begin, end, value);
        if (ec != std::errc{} || ptr != end)
            return std::nullopt;

        return value;
    }
};
