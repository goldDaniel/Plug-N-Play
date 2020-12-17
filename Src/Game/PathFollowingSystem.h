#ifndef PATH_FOLLOWING_SYSTEM_H
#define PATH_FOLLOWING_SYSTEM_H

#include <Core.h>
#include "Components.h"


class PathFollowingSystem : public System
{
public:

    PathFollowingSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            auto& trans = ECS->GetComponent<Transform>(entity);   
            auto& path = ECS->GetComponent<BezierPath>(entity);
            
            if(path.time < 1)
            {
                path.time += path.speed * dt;
            }
            else
            {
                path.time = 1;
            }
            

            Bezier::Point p = path.curve.valueAt(path.time);
            trans.position.x = p.x;
            trans.position.y = p.y;
        }
    }
};


#endif