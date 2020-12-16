#ifndef PLAYER_MOVEMENT_SYSTEM_H
#define PLAYER_MOVEMENT_SYSTEM_H

#include <Core.h>
#include "Components.h"


class MovementSystem : public System
{
public:

    MovementSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            const auto& vel = ECS->GetComponent<Velocity>(entity);
            auto& trans = ECS->GetComponent<Transform>(entity);   

            trans.position += vel.velocity * dt;
        }
    }
};


#endif