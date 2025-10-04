#include "SystemScheduler.h"

#include "System.h"

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
    for (const std::shared_ptr<System> system : ActiveSystems)
    {
        system->Execute();
    }
}
