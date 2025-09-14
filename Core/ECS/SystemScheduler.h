// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <memory>
#include <vector>

class System;

class SystemScheduler
{
public:
    void Register(std::shared_ptr<System> system);
    void Tick();

private:
    std::vector<std::shared_ptr<System>> ActiveSystems;
};
