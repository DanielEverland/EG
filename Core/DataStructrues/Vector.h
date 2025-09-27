// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <cstdint>
#include <type_traits>

template<typename T, typename U>
concept valid_integral_and_floating_point_conversion = requires
{
    requires sizeof(T) >= sizeof(U);
    requires std::is_floating_point_v<T>;
};

template<class T>
struct TemplatedVector
{
    TemplatedVector() = default;
    TemplatedVector(T inX, T inY) : X(inX), Y(inY) { }
    
    template<class U>
    requires valid_integral_and_floating_point_conversion<T, U>
    TemplatedVector(const TemplatedVector<U>& other) : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y))
    {
        // In place instead of GTest as we cannot expect uncompilable code.
        static_assert(!(std::is_same<T, int>() && std::is_same<U, float>()));
        static_assert(!(std::is_same<T, float>() && std::is_same<U, double>()));
    }

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