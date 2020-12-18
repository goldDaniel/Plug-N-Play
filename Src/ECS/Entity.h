#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H


#include <bitset>
#include <cstdint>


using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 4096;

/**
 * @brief A ComponentType will be represented as a bit inside of a signature
 * 
 */
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;


/**
 * @brief A signature is a bitmap that tells us which components an entity has assosiated with it
 * 
 */
using Signature = std::bitset<MAX_COMPONENTS>;




#endif
