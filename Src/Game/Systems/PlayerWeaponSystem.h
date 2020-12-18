#ifndef PLAYER_WEAPON_SYSTEM_H
#define PLAYER_WEAPON_SYSTEM_H

#include <Core.h>
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

                    auto texture = Texture::CreateTexture("Assets/Textures/Bullet.png");

                    float ratio = (float)texture->GetHeight() / (float)texture->GetWidth();
                    float scaleX = 1;
                    float scaleY = ratio;


                    for (int i = -1; i <= 1; i++)
                    {
                        Entity bullet = ECS->CreateEntity();
                     
                        glm::vec2 pos = trans.position + glm::vec2(i/2.f, trans.scale.y / 2.f + scaleY / 2.f);
                        Transform t{ pos, glm::vec2(scaleX, scaleY), -glm::pi<float>() / 2 };
                        ECS->AddComponent<Transform>(bullet, t);
                        
                        ECS->AddComponent<Bullet>(bullet, Bullet({ 0.25f }));
                        
                        Velocity v{ glm::vec2(0, 24) };
                        ECS->AddComponent<Velocity>(bullet, v);
                        
                        Renderable r{ glm::vec4(1.f,1.f,1.f,1.f), texture };
                        ECS->AddComponent<Renderable>(bullet, r);
                    }
                }                
            }
        }
    }
};


#endif