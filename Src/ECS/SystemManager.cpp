#include "SystemManager.h"

#include <cassert>


/**
 * @brief 
 * 
 * @param entity 
 */
void SystemManager::EntityDestroyed(Entity entity)
{
    for(const auto& pair : systems)
    {
        const auto& system = pair.second;

        system->entities.erase(entity);
    }
}

/**
 * @brief 
 * 
 * @param entity 
 * @param entitySignature 
 */
void SystemManager::EntitySignatureChanged(Entity entity, Signature sig)
{
    // Notify each system that an entity's signature changed
    for (auto const& pair : systems)
    {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& system_sig = signatures[type];

        // Entity signature matches system signature - insert into set
        if ((sig & system_sig) == system_sig)
        {
            system->entities.insert(entity);
        }
        // Entity signature does not match system signature - erase from set
        else
        {
            system->entities.erase(entity);
        }
    }
}
