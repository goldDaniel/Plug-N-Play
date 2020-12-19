#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include <Core.h>
#include "Graphics/Texture.h"

class PlayerMovementSystem : public System
{
public:

    PlayerMovementSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            auto& trans = ECS->GetComponent<Transform>(entity);
            auto& vel = ECS->GetComponent<Velocity>(entity);
            const auto& input = ECS->GetComponent<PlayerInput>(entity);   

            glm::vec2 input_velocity(0,0);
        
            float speed = 14;

            if(input.key_left)  input_velocity.x -= 1;
            if(input.key_right) input_velocity.x += 1;

            if(input.key_up)    input_velocity.y += 1;
            if(input.key_down)  input_velocity.y -= 1;

            vel.velocity = input_velocity;
            if(glm::length(input_velocity) > 0)
            {
                vel.velocity = glm::normalize(input_velocity) * speed;
            }

            if (trans.position.x < -Constants::WORLD_WIDTH)
            {
                trans.position.x = -Constants::WORLD_WIDTH;
            }
            if (trans.position.x > Constants::WORLD_WIDTH)
            {
                trans.position.x = Constants::WORLD_WIDTH;
            } 

            if (trans.position.y < -Constants::WORLD_HEIGHT)
            {
                trans.position.y = -Constants::WORLD_HEIGHT;
            }
            if (trans.position.y > Constants::WORLD_HEIGHT)
            {
                trans.position.y = Constants::WORLD_HEIGHT;
            }
        }
    }
};


#endif