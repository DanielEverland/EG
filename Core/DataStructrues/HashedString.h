// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <string>
#include <unordered_map>

class HashedString
{
public:
    HashedString() = default;
    HashedString(const std::string& str)
    {
        auto [iter, success]  = Lookup.try_emplace(str, Cache.size());
        if (success)
        {
            Cache.push_back(str);
        }
        Idx = iter->second;
    }

    operator size_t() const { return Idx; }
    operator std::string() const;

private:
    static inline std::unordered_map<std::string, size_t> Lookup = { };
    static inline std::vector<std::string> Cache = { };
    size_t Idx = SIZE_MAX;
};

template <>
struct std::hash<HashedString>
{
    std::size_t operator()(const HashedString& str) const
    {
        return str;
    }
};