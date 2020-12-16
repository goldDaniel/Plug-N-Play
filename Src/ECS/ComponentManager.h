#ifndef ECS_COMPONENT_MANAGER_H
#define ECS_COMPONENT_MANAGER_H

#include <cassert>
#include <memory>
#include <unordered_map>


#include "ComponentArray.h"
#include "Entity.h"

class ComponentManager
{
private:
    std::unordered_map<const char*, ComponentType> component_types;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays;

    ComponentType next_component_type;

    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
    /**
     * @brief statically casts to the type of ComponentArray
     * 
     * @tparam T 
     * @return std::shared_ptr<ComponentArray<T>> 
     */
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* type = typeid(T).name();

        assert(component_types.find(type) != component_types.end() && "Component not registered.");

        return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type]);
    }


public:
   
    /**
     * @brief Registers a type of component for use in the ECS
     * 
     * @tparam T The data type
     */
    template<typename T>
    void RegisterComponent()
    {
        const char* type = typeid(T).name();

        assert(component_types.find(type) == component_types.end() && "Component already registered");

        component_types.insert({type, next_component_type});
        
        auto comp_arr = std::make_shared<ComponentArray<T>>();
        component_arrays.insert({type, comp_arr});

        next_component_type++;
    }

    /**
     * @brief Get the Component Type 
     * 
     * @tparam T The component we want the type ID for 
     * @return the type ID 
     */
    template<typename T>
    ComponentType GetComponentType()
    {
        const char* type = typeid(T).name();

        assert(component_types.find(type) != component_types.end() && "Component is not registered");

        return component_types[type];
    }

    /**
     * @brief 
     * 
     * @tparam T 
     * @param entity 
     * @param component 
     */
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    /**
     * @brief 
     * 
     * @tparam T 
     * @param entity 
     * @param component 
     */
    template<typename T>
    void RemoveComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->RemoveData(entity, component);
    }


    /**
     * @brief Get the Component Data
     * 
     * @tparam T 
     * @param entity 
     * @return T& 
     */
    template<typename T>
	T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    /**
     * @brief 
     * 
     * @param entity 
     */
    void EntityDestroyed(Entity entity);
};

#endif
