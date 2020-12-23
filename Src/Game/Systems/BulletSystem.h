#ifndef BULLET_SYSTEM_H
#define BULLET_SYSTEM_H

#include <Core.h>

class BulletSystem : public System
{
public:

    BulletSystem(ECSController * const ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        std::vector<Entity> to_remove;

        for(const auto& entity : entities)
        {
            const auto& trans = ECS->GetComponent<Transform>(entity);   
            const auto& bullet = ECS->GetComponent<Bullet>(entity);
            
            if(trans.position.y < -8 || trans.position.y > 8)
            {
                to_remove.push_back(entity);
            }
        }

        for(const auto& entity : to_remove)
        {
            ECS->DestroyEntity(entity);
        }
    }
};


#endif