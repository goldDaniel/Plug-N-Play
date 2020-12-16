#ifndef ECS_COMPONENT_ARRAY_H
#define ECS_COMPONENT_ARRAY_H

#include <cassert>
#include <unordered_map>
#include "Entity.h"


class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
private:

    /**
     * @brief A packed array of components, with the length equal to the maximum allowed entities
     * 
     */
	std::array<T, MAX_ENTITIES> component_array;

	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> entity_to_index_map;

	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> index_to_entity_map;

	// Total size of valid entries in the array.
	std::size_t component_array_size;

public:
    /**
     * @brief Adds 
     * 
     * @param entity The entity to add a component to
     * @param Component the component to add
     */
    void InsertData(Entity entity, T Component)
    {
        assert(entity_to_index_map.find(entity) == entity_to_index_map.end() && "Entity already has component");

        std::size_t index = component_array_size;

        entity_to_index_map[entity] = index;
        index_to_entity_map[index] = entity;
        component_array[index] = Component;

        component_array_size++;
    }

    /**
     * @brief Removes the component from an entity while maintaining packed array
     * 
     * @param entity 
     */
    void RemoveData(Entity entity)
    {
        assert(entity_to_index_map.find(entity) != entity_to_index_map.end() && "Entity does not have component");

        std::size_t index_removed_entity = entity_to_index_map[entity];
        std::size_t index_last_element = component_array_size - 1;
        component_array[index_removed_entity] = component_array[index_last_element];

        Entity entity_last = index_to_entity_map[index_last_element];
        entity_to_index_map[entity_last] = index_removed_entity;
        index_to_entity_map[index_removed_entity] = entity_last;

        entity_to_index_map.erase(entity);
        index_to_entity_map.erase(index_last_element);

        component_array_size--;
    }
    /**
     * @brief Get the Data component for a given entity
     * 
     * @param entity the entity that has the data we want
     * @return T& the data for the entity
     */
    T& GetData(Entity entity)
    {
        assert(entity_to_index_map.find(entity) != entity_to_index_map.end() && "Entity does not have component.");

        std::size_t index = entity_to_index_map[entity];
        return component_array[index];
    }

    /**
     * @brief Removes the data from a given entity
     * 
     * @param entity 
     */
    void EntityDestroyed(Entity entity) override 
    {
        if(entity_to_index_map.find(entity) != entity_to_index_map.end())
        {
            RemoveData(entity);
        }
    }
};


#endif

