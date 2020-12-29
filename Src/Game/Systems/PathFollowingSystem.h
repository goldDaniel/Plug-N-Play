#ifndef PATH_FOLLOWING_SYSTEM_H
#define PATH_FOLLOWING_SYSTEM_H

#include <Core.h>

class PathFollowingSystem : public System
{
private:
    
    float elapsed = 0;

public:

    PathFollowingSystem(ECSController * const ECS) : System::System(ECS) {}
    
    void SetElapsed(float elapsed)
    {
        this->elapsed = elapsed;

        for (const auto& entity : entities)
        {
            auto& trans = ECS->GetComponent<Transform>(entity);
            auto& path = ECS->GetComponent<BezierPath>(entity);

            if (elapsed < path.time_start)
            {
                path.time = 0;
                auto p = path.curve.valueAt(0);
                trans.position = { p.x, p.y };
            }
            else //if (elapsed >= path.time_start)
            {
                //calculate the percentage of the path we have gone through and 
                //set the time appropriately
                path.time = (elapsed - path.time_start) * path.speed;
                if (path.time > 1) path.time = 1;

                auto p = path.curve.valueAt(path.time);
                trans.position = { p.x, p.y };
            }
        }
    }

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
                    path.time = 1;

                    //path following is complete, destroy the entity
                    to_remove.insert(entity);
                }
            }
        }

        for (const auto& entity : to_remove)
        {
            //We arent destroying entities because of the path editor, we must keep 
            //path followers in the system in order to scrub through the level

            //ECS->DestroyEntity(entity);
        }
    }
};


#endif