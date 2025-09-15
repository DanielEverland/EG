// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <string>
#include <system_error>

class HashedString
{
public:
    constexpr HashedString(const std::string& str)
    {
        // https://stackoverflow.com/a/7666577/3834696
        const char* cStr = str.c_str();
        Hash = 5381;
        int c;

        while (c = *cStr++)
        {
            Hash = ((Hash << 5) + Hash) + c;
        }        
    }

    operator size_t() const { return Hash; }

private:
    size_t Hash = -1;
};

template <>
struct std::hash<HashedString>
{
    std::size_t operator()(const HashedString& str) const
    {
        return str;
    }
};