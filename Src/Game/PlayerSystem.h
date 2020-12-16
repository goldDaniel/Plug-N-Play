#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include <Core.h>
#include "Components.h"


class PlayerSystem : public System
{
private:
    

public:

    PlayerSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            auto& transform = ECS->GetComponent<Transform>(entity);
            const auto& input = ECS->GetComponent<PlayerInput>(entity);   

            glm::vec2 velocity(0,0);
            
            if(input.key_left)
            {
                velocity.x -= 8;
            }
            if(input.key_right)
            {
                velocity.x += 8;
            }
            if(input.key_up)
            {
                velocity.y += 8;
            }
            if(input.key_down)
            {
                velocity.y -= 8;
            }

            transform.position += velocity * dt;
        }
    }
};


#endif