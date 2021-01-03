#ifndef ECS_CONTROLLER_H
#define ECS_CONTROLLER_H

#include <memory>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class ECSController
{
private:
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<SystemManager> system_manager;

public:
    ECSController();

    //ENTITY========================================================
    Entity CreateEntity();
    void DestroyEntity(Entity e);

    //COMPONENT========================================================
    template<typename T>
    void RegisterComponent()
    {
        component_manager->RegisterComponent<T>();
    }

    template<typename T>
    bool HasComponent(Entity entity)
    {
        auto entity_sig = entity_manager->GetSignature(entity);
        auto component_flag = component_manager->GetComponentType<T>();
        
        return entity_sig.test(component_flag);
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        component_manager->AddComponent<T>(entity, component);

        auto sig = entity_manager->GetSignature(entity);
        sig.set(component_manager->GetComponentType<T>(), true);
        entity_manager->SetSignature(entity, sig);

        system_manager->EntitySignatureChanged(entity, sig);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        component_manager->RemoveComponent<T>(entity);

        auto sig = entity_manager->GetSignature(entity);
        sig.set(component_manager->GetComponentType<T>(), false);
        entity_manager->SetSignature(entity, sig);

        system_manager->EntitySignatureChanged(entity, sig);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return component_manager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return component_manager->GetComponentType<T>();
    }


    //SYSTEM========================================================
	template<typename T>
    std::shared_ptr<T> RegisterSystem(Signature sig)
    {
        auto system = system_manager->RegisterSystem<T>(this);
        
        system_manager->SetSignature<T>(sig);

        return system;
    }
};


#endif
