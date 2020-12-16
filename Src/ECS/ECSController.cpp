#include "ECSController.h"

ECSController::ECSController()
{
    entity_manager = std::make_unique<EntityManager>();
    component_manager = std::make_unique<ComponentManager>();
    system_manager = std::make_unique<SystemManager>();
}

Entity ECSController::CreateEntity()
{
    return entity_manager->CreateEntity();
}

void ECSController::DestroyEntity(Entity e)
{
    entity_manager->DestroyEntity(e);
    component_manager->EntityDestroyed(e);
    system_manager->EntityDestroyed(e);
}