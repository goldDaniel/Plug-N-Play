#include "Application.h"

#include "Core.h"

#include "Graphics/Texture.h"

#include "Game/Components.h"
#include "Game/Systems/BulletSystem.h"
#include "Game/Systems/InputSystem.h"
#include "Game/Systems/CameraSystem.h"
#include "Game/Systems/MovementSystem.h"
#include "Game/Systems/PathFollowingSystem.h"
#include "Game/Systems/PlayerWeaponSystem.h"
#include "Game/Systems/PlayerMovementSystem.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/DebugRenderSystem.h"


Application::Application()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    window_width = 576; //DM.w;
    window_height = 1024; //DM.h;

    window = SDL_CreateWindow( "Plug N Play", 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                window_width, window_height, 
                                SDL_WINDOW_OPENGL);


    context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);


#ifndef __arm__
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
#endif
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application::~Application()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::Run()
{
    bool running = true;

    ECSController ECS;
    ECS.RegisterComponent<Transform>();
    ECS.RegisterComponent<Velocity>();
    ECS.RegisterComponent<Camera>();
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
        cam_comp.proj = glm::perspective(glm::radians(67.f), (float)window_width/(float)window_height, 1.f, 100.f);;
        cam_comp.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
        ECS.AddComponent(camera, cam_comp);


        debugRenderSystem->SetCamera(camera);
        renderSystem->SetCamera(camera);    
    }
    
    //creates the player entity
    {
        auto texture = Texture::CreateTexture("Assets/Textures/Player.png");
        Entity player = ECS.CreateEntity();  
        ECS.AddComponent(player, Transform{glm::vec2(0, -5), glm::vec2(1.5f, 1.5f), 0.f});
        ECS.AddComponent(player, Velocity());
        ECS.AddComponent(player, PlayerInput());
        ECS.AddComponent(player, InputSet{SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_SPACE});
        ECS.AddComponent(player, Weapon({0.1f}));
        ECS.AddComponent(player, Renderable{glm::vec4(1,1,1,1), texture });        
        ECS.AddComponent(player, DebugRenderable{DebugRenderable::ShapeType::CIRCLE, glm::vec4(1,0,0,1)});        
    }

    //creates the enemy entity
    {
        Entity enemy = ECS.CreateEntity();
        ECS.AddComponent(enemy, Transform{glm::vec2(0,0), glm::vec2(1.f, 1.f), 0.f});

        Bezier::Bezier<2> curve({{-3.f, 6.f}, {3.f, 6.f}, {3.f, -6.f}});
        float speed = 1.f/4.f;
        BezierPath path({curve, speed, 0.f});

        auto tex = Texture::CreateTexture("Assets/Textures/Enemy.png");
        ECS.AddComponent(enemy, path);
        ECS.AddComponent(enemy, Renderable({glm::vec4(1,1,1,1), tex }));
    }
    //creates the enemy entity
    {
        Entity enemy = ECS.CreateEntity();
        ECS.AddComponent(enemy, Transform{glm::vec2(0,0), glm::vec2(1.f, 1.f), 0.f});

        Bezier::Bezier<2> curve({{3.f, 6.f}, {-3.f, 6.f}, {-3.f, -6.f}});
        float speed = 1.f/4.f;
        BezierPath path({curve, speed, 0.f});

        auto tex = Texture::CreateTexture("Assets/Textures/Enemy.png");
        ECS.AddComponent(enemy, path);
        ECS.AddComponent(enemy, Renderable({glm::vec4(1,1,1,1), tex }));
    }

    float elapsed = 0;
    float prevTicks = SDL_GetTicks();
    
    while(running)
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
        cameraSystem->Update(dt);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderSystem->Update(dt);
        debugRenderSystem->Update(dt);

        SDL_GL_SwapWindow(window);
    }
}
