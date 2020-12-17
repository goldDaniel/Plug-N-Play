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
            

            glm::vec2 pos_previous = trans.position;

            Bezier::Point p_current = path.curve.valueAt(path.time);
            trans.position.x = p_current.x;
            trans.position.y = p_current.y;

            glm::vec2 dir = trans.position - pos_previous;
            trans.rotation = glm::atan(dir.y, dir.x);
        }
    }
};


#endif