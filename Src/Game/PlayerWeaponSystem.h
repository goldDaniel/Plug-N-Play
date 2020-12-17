#ifndef PLAYER_WEAPON_SYSTEM_H
#define PLAYER_WEAPON_SYSTEM_H

#include <Core.h>
#include "Components.h"
#include "Graphics/Texture.h"


class PlayerWeaponSystem : public System
{
public:

    PlayerWeaponSystem(ECSController* ECS) : System::System(ECS) {}
    
    void Update(float dt)
    {
        for(const auto& entity : entities)
        {
            const auto& trans = ECS->GetComponent<Transform>(entity);
            const auto& input = ECS->GetComponent<PlayerInput>(entity);
            auto& weapon = ECS->GetComponent<Weapon>(entity);   

            if(weapon.cooldown_timer > 0)
            {
                weapon.cooldown_timer -= dt;
            }

            if(input.key_shoot)
            {
                if(weapon.cooldown_timer <= 0)
                {
                    weapon.cooldown_timer = weapon.cooldown;

                    Entity bullet = ECS->CreateEntity();

                    auto texture = Texture::CreateTexture("Assets/Textures/Bullet.png").get();

                    float ratio = (float)texture->GetWidth() / (float)texture->GetHeight();
                    float scaleX = ratio;
                    float scaleY = 1;

                    glm::vec2 pos = trans.position + glm::vec2(0, trans.scale.y / 2.f + scaleY / 2.f);
                    Transform t { pos, glm::vec2(scaleX, scaleY), 0 };
                    ECS->AddComponent<Transform>(bullet, t);

                    ECS->AddComponent<Bullet>(bullet, Bullet({0.25f}));

                    Velocity v { glm::vec2(0, 24) };
                    ECS->AddComponent<Velocity>(bullet, v); 

                    Renderable r { glm::vec4(1.f,1.f,1.f,1.f), texture };
                    ECS->AddComponent<Renderable>(bullet, r);
                }                
            }
        }
    }
};


#endif