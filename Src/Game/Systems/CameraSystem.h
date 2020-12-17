#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <Core.h>

class CameraSystem : public System
{

public:

    CameraSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            auto& cam = ECS->GetComponent<Camera>(entity);   
        }
    }
};


#endif