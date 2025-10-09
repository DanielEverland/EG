// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

class System;

class ScopedSystemWriteLock
{
public:
    ScopedSystemWriteLock(const System* toLock);
    ~ScopedSystemWriteLock();

private:
    const System* const LockedSystem;
};
