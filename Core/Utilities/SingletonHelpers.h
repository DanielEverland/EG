// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <thread>

namespace SingletonHelper
{
    template <class T>
    T& Impl()
    {
        static T* instance = nullptr;
        if (instance == nullptr)
        {
            instance = new T();
        }
        return *instance;
    }
}
