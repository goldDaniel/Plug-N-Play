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
        ECS.RegisterComponent<Transform>();
        ECS.RegisterComponent<Velocity>();
        ECS.RegisterComponent<Camera>();
        ECS.RegisterComponent<Collider>();
        ECS.RegisterComponent<Collision>();
        ECS.RegisterComponent<Bullet>();
        ECS.RegisterComponent<Weapon>();
        ECS.RegisterComponent<BezierPath>();
        ECS.RegisterComponent<InputSet>();
        ECS.RegisterComponent<PlayerInput>();
        ECS.RegisterComponent<Renderable>();
        ECS.RegisterComponent<DebugRenderable>();


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
            cam_comp.proj = glm::perspective(glm::radians(67.f), (float)window_width / (float)window_height, 1.f, 100.f);;
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

        //creates the enemy entity
        {
            Entity enemy = ECS.CreateEntity();
            ECS.AddComponent(enemy, Transform{ glm::vec2(0,0), glm::vec2(1.f, 1.f), 0.f });

            Bezier::Bezier<2> curve({ {-4.5f, 6.f}, {3.f, 6.f}, {3.5f, -8.f} });
            float speed = 1.f / 4.f;
            BezierPath path({ curve, speed, 0.f });

            auto tex = Texture::CreateTexture("Assets/Textures/Enemy.png");
            ECS.AddComponent(enemy, path);
            ECS.AddComponent(enemy, Collider{ Collider::Enemy, Collider::Player, 0.5f });
            ECS.AddComponent(enemy, Renderable({ glm::vec4(1,1,1,1), tex }));
        }
        //creates the enemy entity
        {
            Entity enemy = ECS.CreateEntity();
            ECS.AddComponent(enemy, Transform{ glm::vec2(0,0), glm::vec2(1.f, 1.f), 0.f });

            Bezier::Bezier<2> curve({ {4.5f, 6.f}, {-3.f, 6.f}, {-3.5f, -8.f} });
            float speed = 1.f / 4.f;
            BezierPath path({ curve, speed, 0.f });

            auto tex = Texture::CreateTexture("Assets/Textures/Enemy.png");
            ECS.AddComponent(enemy, path);
            ECS.AddComponent(enemy, Collider{ Collider::Enemy, Collider::Player, 0.5f });
            ECS.AddComponent(enemy, Renderable({ glm::vec4(1,1,1,1), tex }));
        }

        float elapsed = 0;
        float prevTicks = SDL_GetTicks();

        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

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

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderSystem->Update(dt);
            debugRenderSystem->Update(dt);

            SDL_GL_SwapWindow(window);
        }
	}
};

#endif