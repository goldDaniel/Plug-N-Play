
#include "EntityManager.h"

#include <cassert>
#include <iostream>

/**
 * @brief Construct a new Entity Manager object
 * 
 */
EntityManager::EntityManager()
{
    for(Entity e = 0; e < MAX_ENTITIES; e++)
    {
        available_entities.push(e);
    }
}

/**
 * @brief Create an Entity with no components
 * 
 * @return Entity that was created
 */
Entity EntityManager::CreateEntity()
{
    assert(current_entity_count < MAX_ENTITIES && "Too many entities");

    Entity id = available_entities.front();
    available_entities.pop();

    current_entity_count++;

    return id;
}

/**
 * @brief Destroys an entity
 * 
 * @param e entity to destroy
 */
void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Invalid Entity value");

    //reset components the entity has
    entity_signatures[entity].reset();
    available_entities.push(entity);

    current_entity_count--;
}

/**
 * @brief Sets the component bitfield for the given entity
 * 
 * @param entity the entity we are altering the signature of
 * @param sig the signature to apply to the entity
 */
void EntityManager::SetSignature(Entity entity, Signature sig)
{
    assert(entity < MAX_ENTITIES && "Invalid Entity value");
    entity_signatures[entity] = sig;
}

/**
 * @brief Get the Signature for a given entity
 * 
 * @param entity the entity we are getting the signature of 
 * @return Signature the signature of the given entity
 */
Signature EntityManager::GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Invalid Entity value");

    return entity_signatures[entity];
}
