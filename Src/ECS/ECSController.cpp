#include "ECSController.h"

#include <Game/Components.h>

ECSController::ECSController()
{
    entity_manager = std::make_unique<EntityManager>();
    component_manager = std::make_unique<ComponentManager>();
    system_manager = std::make_unique<SystemManager>();

    RegisterComponent<Transform>();
    RegisterComponent<Velocity>();
    RegisterComponent<Camera>();
    RegisterComponent<Collider>();
    RegisterComponent<Collision>();
    RegisterComponent<Bullet>();
    RegisterComponent<Weapon>();
    RegisterComponent<BezierPath>();
    RegisterComponent<InputSet>();
    RegisterComponent<PlayerInput>();
    RegisterComponent<Renderable>();
    RegisterComponent<DebugRenderable>();
}

Entity ECSController::CreateEntity()
{
    return entity_manager->CreateEntity();
}

Entity ECSController::CloneEntity(Entity entity)
{

    //TODO: clone entity data, not just component types

    Entity clone = entity_manager->CreateEntity();
    auto signature = entity_manager->GetSignature(entity);
    
    entity_manager->SetSignature(clone, signature);
    system_manager->EntitySignatureChanged(clone, signature);

    
    return clone;
}

void ECSController::DestroyEntity(Entity e)
{
    entity_manager->DestroyEntity(e);
    component_manager->EntityDestroyed(e);
    system_manager->EntityDestroyed(e);
}