// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>
#include <cstdio>
#include <functional>

struct GenericHandle
{
    GenericHandle();

    bool IsValid() const { return Id != 0; }
    
    operator bool() const { return IsValid(); }
    operator std::size_t() const { return Id; }

    bool operator==(const GenericHandle& other) const
    {
        return Id == other.Id;
    }
    bool operator!=(const GenericHandle& other) const
    {
        return Id != other.Id;
    }


private:
    uint32_t Id = 0;
};

template<>
struct std::hash<GenericHandle>
{
    std::size_t operator()(const GenericHandle& handle) const
    {
        return handle;
    }
};