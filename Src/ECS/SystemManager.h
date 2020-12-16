#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H


#include <cassert>
#include <memory>
#include <set>
#include <unordered_map>

#include "Entity.h"

class ECSController;

class System
{
public:
    ECSController * const ECS;
    std::set<Entity> entities;
    
    System( ECSController * const  ECS): ECS(ECS) {}
};

class SystemManager
{
public: 
    
    /**
     * @brief 
     * 
     * @tparam T 
     * @return std::shared_ptr<T> 
     */
    template<typename T>
    std::shared_ptr<T> RegisterSystem(ECSController* ECS)
    {
        const char* type = typeid(T).name();

        assert(systems.find(type) == systems.end() && "System already registered");

        auto system = std::make_shared<T>(ECS);
        systems.insert({type, system});

        return system;
    }

    /**
     * @brief Set the Signature for a given system
     * 
     * @tparam T The system we want to set the signature for
     * @param sig the signature for the system
     */
    template<typename T>
    void SetSignature(Signature sig)
    {
        const char* type = typeid(T).name();

        assert(systems.find(type) != systems.end() && "System is not registered");

        signatures.insert({type, sig});
    }

    /**
     * @brief 
     * 
     * @param entity 
     */
    void EntityDestroyed(Entity entity);

    /**
     * @brief 
     * 
     * @param entity 
     * @param entitySignature 
     */
    void EntitySignatureChanged(Entity entity, Signature sig);

private: 
    std::unordered_map<const char*, Signature> signatures;
    std::unordered_map<const char*, std::shared_ptr<System>> systems;
};

#endif
