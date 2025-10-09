// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <list>
#include <memory>
#include <queue>
#include <shared_mutex>
#include <vector>

class System;

class SystemScheduler
{
public:
    SystemScheduler();
    ~SystemScheduler();
    
    void Register(std::shared_ptr<System> system);
    void Tick();
    void WorkerTick();

private:
    std::atomic_bool running;
    std::condition_variable QueueCondition;
    std::condition_variable EmptyCondition;
    std::mutex QueueMutex;
    std::queue<std::shared_ptr<System>> SystemsToTick;
    std::vector<std::thread> Workers;
    std::vector<std::shared_ptr<System>> ActiveSystems;
};
