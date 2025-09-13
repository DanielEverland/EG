#include "SystemScheduler.h"

void SystemScheduler::Register(std::shared_ptr<System> system)
{
    if (ActiveSystems.empty())
    {
        ActiveSystems.push_back(system);
        return;
    }
    
    for (auto iter = ActiveSystems.begin(); iter != ActiveSystems.end(); ++iter)
    {
        if ((*iter)->GetPriority() > system->GetPriority())
        {
            ActiveSystems.insert(iter, system);
            return;
        }
    }
}

void SystemScheduler::Tick()
{
    // TODO: Improve by executing systems in parallel that do not depend on each other
    for (const std::shared_ptr<System> system : ActiveSystems)
    {
        system->Execute();
    }
}
