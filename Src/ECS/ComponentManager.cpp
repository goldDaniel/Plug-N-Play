#include "ComponentManager.h"

/**
 * @brief 
 * 
 * @param entity 
 */
void ComponentManager::EntityDestroyed(Entity entity)
{
    // Notify each component array that an entity has been destroyed
    for (const auto& pair : component_arrays)
    {
        const auto& component = pair.second;
        component->EntityDestroyed(entity);
    }
}