#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <Core.h>
#include <functional>

class InputSystem : public System
{
private:
    std::unordered_map<int, bool> keys_pressed;
    
    std::function<void()> quit_callback;


public:
    InputSystem(ECSController* ECS) : System::System(ECS) 
    {
        for(int i = 'a'; i < 'z'; i++)
        {
            keys_pressed[i] = false;
        }
    }

    void SetQuitCallback(std::function<void()> callback)
    {
        this->quit_callback = callback;
    }

    void Update(float dt)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_KEYUP)
            {
                keys_pressed[e.key.keysym.sym] = false;
            }
            if(e.type == SDL_KEYDOWN)
            {
                keys_pressed[e.key.keysym.sym] = true;

                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit_callback();
                }    
            }
        }
        for(const auto& entity : entities)
        {
            const auto& inputSet = ECS->GetComponent<InputSet>(entity);
            auto& input = ECS->GetComponent<PlayerInput>(entity);

            input.key_left = keys_pressed[inputSet.key_left];
            input.key_right = keys_pressed[inputSet.key_right];
            input.key_up = keys_pressed[inputSet.key_up];
            input.key_down = keys_pressed[inputSet.key_down];
        }   
    }
};

#endif