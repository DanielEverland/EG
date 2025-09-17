// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <cstdint>

template<class T>
struct TemplatedVector
{
    TemplatedVector() = default;
    TemplatedVector(T inX, T inY) : X(inX), Y(inY) { }
    
    T X = 0;
    T Y = 0;

    TemplatedVector operator+(const TemplatedVector& rhs) const
    {
        return TemplatedVector(X + rhs.X, Y + rhs.Y);
    }

    TemplatedVector operator-(const TemplatedVector& rhs) const
    {
        return TemplatedVector(X - rhs.X, Y - rhs.Y);
    }

    bool operator==(const TemplatedVector& other) const
    {
        return X == other.X && Y == other.Y;
    }
};

// Common types
typedef TemplatedVector<int32_t> IntVector2D;
typedef TemplatedVector<float> Vector2D;