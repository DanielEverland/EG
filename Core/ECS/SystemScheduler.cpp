#include "SystemScheduler.h"

#include "System.h"
#include "ScopedSystemWriteLock.h"

SystemScheduler::SystemScheduler()
{
    running = true;
    uint8_t threads = std::thread::hardware_concurrency();
    for (int i = 0; i < threads; ++i)
    {
        Workers.push_back(std::thread(&SystemScheduler::WorkerTick, this));
    }
}

SystemScheduler::~SystemScheduler()
{
    running = false;
    QueueCondition.notify_all();
}

void SystemScheduler::Register(std::shared_ptr<System> system)
{
    for (auto iter = ActiveSystems.begin(); iter != ActiveSystems.end(); ++iter)
    {
        if ((*iter)->GetPriority() > system->GetPriority())
        {
            ActiveSystems.insert(iter, system);
            return;
        }
    }
    ActiveSystems.push_back(system);
}

void SystemScheduler::Tick()
{
    // TODO: Implement ordering by letting systems declare dependencies on each other
    // TODO: Improve by executing systems in parallel that do not depend on each other

    // TODO CONC: Split all systems into separate threads using a thread pool. Each thread executing several systems in sequence
    // TODO CONC 2: Do this in a smart way so we schedule systems that do not lock each other

    {
        std::scoped_lock lock(QueueMutex);
        for (const std::shared_ptr<System> system : ActiveSystems)
        {
            SystemsToTick.push(system);
        }
    }
    QueueCondition.notify_all();
    std::unique_lock lock(QueueMutex);
    EmptyCondition.wait(lock, [this] { return SystemsToTick.empty(); });
}

void SystemScheduler::WorkerTick()
{
    while (running)
    {
        std::shared_ptr<System> toTick = nullptr;
        {
            std::unique_lock lock(QueueMutex);
            QueueCondition.wait(lock, [this] { return !SystemsToTick.empty(); });
            if (SystemsToTick.empty() || !running)
                continue;
            
            toTick = SystemsToTick.front();
            SystemsToTick.pop();
        }

        {
            auto lock = ScopedSystemWriteLock(toTick.get());
            toTick->Execute();
        }

        EmptyCondition.notify_all();
    }
}
