#ifndef PATH_FOLLOWING_SYSTEM_H
#define PATH_FOLLOWING_SYSTEM_H

#include <Core.h>

class PathFollowingSystem : public System
{
public:

    PathFollowingSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        std::set<Entity> to_remove;
        for(const auto& entity : entities)
        {
            auto& trans = ECS->GetComponent<Transform>(entity);   
            auto& path = ECS->GetComponent<BezierPath>(entity);

            
            if(path.time < 1)
            {
                path.time += path.speed * dt;

                Bezier::Point p_current = path.curve.valueAt(path.time);
                trans.position.x = p_current.x;
                trans.position.y = p_current.y;
            }
            else
            {
                //path following is complete, destroy the entity
                to_remove.insert(entity);
            }
        }

        for (const auto& entity : to_remove)
        {
            ECS->DestroyEntity(entity);
        }
    }
};


#endif