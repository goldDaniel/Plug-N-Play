#ifndef ECS_ENTITY_MANAGER_H
#define ECS_ENTITY_MANAGER_H

#include <array>
#include <bitset>
#include <queue>

#include "Entity.h"

/**
 * @brief Manages the creation and destruction of entities
 * 
 */
class EntityManager
{
private:
    std::queue<Entity> available_entities;
    std::array<Signature, MAX_ENTITIES> entity_signatures;

    std::uint32_t current_entity_count = 0;

public:

    /**
     * @brief Construct a new Entity Manager object
     * 
     */
    EntityManager();

    /**
     * @brief Create an Entity with no components
     * 
     * @return Entity that was created
     */
    Entity CreateEntity();

    /**
     * @brief Destroys an entity
     * 
     * @param e entity to destroy
     */
    void DestroyEntity(Entity entity);

    /**
     * @brief Sets the component bitfield for the given entity
     * 
     * @param entity the entity we are altering the signature of
     * @param sig the signature to apply to the entity
     */
    void SetSignature(Entity entity, Signature sig);

    /**
     * @brief Get the Signature for a given entity
     * 
     * @param entity the entity we are getting the signature of 
     * @return Signature the signature of the given entity
     */
    Signature GetSignature(Entity entity);
};

#endif