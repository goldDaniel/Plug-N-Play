#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include <Application.h>

#include <Core.h>

#include <Graphics/Texture.h>

#include <Game/Components.h>
#include <Game/Systems/BulletSystem.h>
#include <Game/Systems/InputSystem.h>
#include <Game/Systems/CameraSystem.h>
#include <Game/Systems/CollisionSystem.h>
#include <Game/Systems/CollisionResolutionSystem.h>
#include <Game/Systems/MovementSystem.h>
#include <Game/Systems/PathFollowingSystem.h>
#include <Game/Systems/PlayerWeaponSystem.h>
#include <Game/Systems/PlayerMovementSystem.h>
#include <Game/Systems/RenderSystem.h>
#include <Game/Systems/DebugRenderSystem.h>


//TODO: refactor into a shared folder 
#include <Editor/data/StagePersistence.h>


class GameApplication : public Application
{
public:
	GameApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height) : 
        Application::Application(window, context, window_width, window_height) {}

	virtual ~GameApplication() {}

	virtual void Run() override
	{
        bool running = true;

        ECSController ECS;

        Signature inputSig;
        inputSig.set(ECS.GetComponentType<InputSet>());
        inputSig.set(ECS.GetComponentType<PlayerInput>());
        auto inputSystem = ECS.RegisterSystem<InputSystem>(inputSig);
        inputSystem->SetQuitCallback([&running]()
        {
            running = false;
        });


        Signature playerMoveSig;
        playerMoveSig.set(ECS.GetComponentType<Transform>());
        playerMoveSig.set(ECS.GetComponentType<Velocity>());
        playerMoveSig.set(ECS.GetComponentType<PlayerInput>());
        auto playerSystem = ECS.RegisterSystem<PlayerMovementSystem>(playerMoveSig);


        Signature playerWeaponSig;
        playerWeaponSig.set(ECS.GetComponentType<Transform>());
        playerWeaponSig.set(ECS.GetComponentType<Weapon>());
        playerWeaponSig.set(ECS.GetComponentType<PlayerInput>());
        auto weaponSystem = ECS.RegisterSystem<PlayerWeaponSystem>(playerWeaponSig);


        Signature movementSig;
        movementSig.set(ECS.GetComponentType<Transform>());
        movementSig.set(ECS.GetComponentType<Velocity>());
        auto movementSystem = ECS.RegisterSystem<MovementSystem>(movementSig);


        Signature camSig;
        camSig.set(ECS.GetComponentType<Camera>());
        auto cameraSystem = ECS.RegisterSystem<CameraSystem>(camSig);


        Signature bulletSig;
        bulletSig.set(ECS.GetComponentType<Transform>());
        bulletSig.set(ECS.GetComponentType<Bullet>());
        auto bulletSystem = ECS.RegisterSystem<BulletSystem>(bulletSig);


        Signature pathSig;
        pathSig.set(ECS.GetComponentType<Transform>());
        pathSig.set(ECS.GetComponentType<BezierPath>());
        auto pathSystem = ECS.RegisterSystem<PathFollowingSystem>(pathSig);

        Signature collisionDetectionSig;
        collisionDetectionSig.set(ECS.GetComponentType<Transform>());
        collisionDetectionSig.set(ECS.GetComponentType<Collider>());
        auto collisionSystem = ECS.RegisterSystem<CollisionSystem>(collisionDetectionSig);

        Signature resolutionSig;
        resolutionSig.set(ECS.GetComponentType<Collision>());
        auto resolutionSystem = ECS.RegisterSystem<CollisionResolutionSystem>(resolutionSig);


        Signature debugSig;
        debugSig.set(ECS.GetComponentType<Transform>());
        debugSig.set(ECS.GetComponentType<DebugRenderable>());
        auto debugRenderSystem = ECS.RegisterSystem<DebugRenderSystem>(debugSig);
        debugRenderSystem->SetScreenDimensions(window_width, window_height);


        Signature renderSig;
        renderSig.set(ECS.GetComponentType<Transform>());
        renderSig.set(ECS.GetComponentType<Renderable>());
        auto renderSystem = ECS.RegisterSystem<RenderSystem>(renderSig);
        renderSystem->SetScreenDimensions(window_width, window_height);

        //creates the camera entity
        {
            Entity camera = ECS.CreateEntity();
            Camera cam_comp;

            //because this is not the editor, we want to assure that we only see the game world
            //We want a 9:16 aspect ratio ideally, so we flip the width and height 
            if (window_width < window_height)
            {
                cam_comp.proj = glm::perspective(glm::radians(67.f), (float)window_width / (float)window_height, 1.f, 100.f);
            }
            else
            {
                cam_comp.proj = glm::perspective(glm::radians(67.f), (float)window_height / (float)window_width, 1.f, 100.f);
            }
            
            
            cam_comp.view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            ECS.AddComponent(camera, cam_comp);


            debugRenderSystem->SetCamera(camera);
            renderSystem->SetCamera(camera);
        }

        //creates the player entity
        {
            auto texture = Texture::CreateTexture("Assets/Textures/Player.png");
            Entity player = ECS.CreateEntity();
            ECS.AddComponent(player, Transform{ glm::vec2(0, -5), glm::vec2(1.5f, 1.5f), 0.f });
            ECS.AddComponent(player, Velocity());
            ECS.AddComponent(player, PlayerInput());
            ECS.AddComponent(player, InputSet{ SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_SPACE });
            ECS.AddComponent(player, Weapon({ 0.1f }));
            ECS.AddComponent(player, Collider{ Collider::Player, Collider::None, 0.1f });
            ECS.AddComponent(player, Renderable{ glm::vec4(1,1,1,1), texture });
        }

        auto persistence = std::make_unique<StagePersistence>();
        
        persistence->LoadStage(&ECS, "Assets/Stages/Testing Stage.stage");

        float elapsed = 0;
        float prevTicks = SDL_GetTicks();

        while (running)
        {
            SDL_GetWindowSize(window, &window_width, &window_height);

            float currTime = SDL_GetTicks();
            float dt = (currTime - prevTicks) / 1000.f;
            prevTicks = currTime;
            elapsed += dt;


            inputSystem->Update(dt);
            playerSystem->Update(dt);
            movementSystem->Update(dt);
            weaponSystem->Update(dt);
            pathSystem->Update(dt);
            bulletSystem->Update(dt);
            collisionSystem->Update(dt);
            resolutionSystem->Update(dt);
            cameraSystem->Update(dt);

            float ratio;
            if (window_width < window_height)
            {
                ratio = (float)window_width / (float)window_height;
            }
            else
            {
                ratio = (float)window_height / (float)window_width;
            }
            float viewport_width = window_height * ratio;
            float viewport_x = (window_width - viewport_width) / 2.f;
            glViewport(viewport_x, 0, viewport_width, window_height);

            
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderSystem->Update(dt);
            debugRenderSystem->Update(dt);

            SDL_GL_SwapWindow(window);
        }
	}
};

#endif