#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include <Core.h>
#include "Components.h"
#include "Graphics/Texture.h"


class PlayerMovementSystem : public System
{
public:

    PlayerMovementSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            auto& vel = ECS->GetComponent<Velocity>(entity);
            const auto& input = ECS->GetComponent<PlayerInput>(entity);   

            glm::vec2 input_velocity(0,0);
        
            if(input.key_left)
            {
                input_velocity.x -= 8;
            }
            if(input.key_right)
            {
                input_velocity.x += 8;
            }
            if(input.key_up)
            {
                input_velocity.y += 8;
            }
            if(input.key_down)
            {
                input_velocity.y -= 8;
            }

            vel.velocity = input_velocity;
        }
    }
};


#endif