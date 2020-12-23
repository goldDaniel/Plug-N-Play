#ifndef PATH_FOLLOWING_SYSTEM_H
#define PATH_FOLLOWING_SYSTEM_H

#include <Core.h>

class PathFollowingSystem : public System
{
private:
    
    float elapsed = 0;

public:

    PathFollowingSystem(ECSController * const ECS) : System::System(ECS) {}
    

    void Update(float dt)
    {
        elapsed += dt;

        std::set<Entity> to_remove;
        for(const auto& entity : entities)
        {
            auto& trans = ECS->GetComponent<Transform>(entity);   
            auto& path = ECS->GetComponent<BezierPath>(entity);

            if (elapsed >= path.time_start)
            {
                if (path.time < 1)
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
        }

        for (const auto& entity : to_remove)
        {
            ECS->DestroyEntity(entity);
        }
    }
};


#endif