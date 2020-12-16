#include "Application.h"

#include "Game/Components.h"
#include "Game/InputSystem.h"
#include "Game/CameraSystem.h"
#include "Game/PlayerSystem.h"
#include "Game/DebugRenderSystem.h"


Application::Application()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow( "Plug N Play", 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                window_width, window_height, 
                                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);


    context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);


    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    window_width = DM.w;
    window_height = DM.h;
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
    ECS.RegisterComponent<Camera>();
    ECS.RegisterComponent<InputSet>();
    ECS.RegisterComponent<PlayerInput>();
    ECS.RegisterComponent<DebugRenderable>();

    auto inputSystem = ECS.RegisterSystem<InputSystem>();
    inputSystem->SetQuitCallback([&running]() 
    {
        running = false;
    });

    Signature inputSig;
    inputSig.set(ECS.GetComponentType<InputSet>());
    inputSig.set(ECS.GetComponentType<PlayerInput>());
    ECS.SetSystemSignature<InputSystem>(inputSig);

    auto playerSystem = ECS.RegisterSystem<PlayerSystem>();
    Signature playerSig;
    playerSig.set(ECS.GetComponentType<Transform>());
    playerSig.set(ECS.GetComponentType<PlayerInput>());
    ECS.SetSystemSignature<PlayerSystem>(playerSig);

    auto cameraSystem = ECS.RegisterSystem<CameraSystem>();
    Signature camSig;
    camSig.set(ECS.GetComponentType<Camera>());
    ECS.SetSystemSignature<CameraSystem>(camSig);

    auto renderSystem = ECS.RegisterSystem<DebugRenderSystem>();
    renderSystem->SetScreenDimensions(window_width, window_height);
    Signature renderSig;
    renderSig.set(ECS.GetComponentType<Transform>());
    renderSig.set(ECS.GetComponentType<DebugRenderable>());
    ECS.SetSystemSignature<DebugRenderSystem>(renderSig);


    int entites_sqrt = 32;
    for(int i = 0; i < entites_sqrt; i++)
    {
        for(int j = 0; j < entites_sqrt; j++)
        {
            float posX = (float)(i + 1) / (float)entites_sqrt * 20.f - 10.f;
            float posY = (float)(j + 1) / (float)entites_sqrt * 16.f - 8.f;
    
    
            Entity e = ECS.CreateEntity();  
            ECS.AddComponent(e, Transform{glm::vec2(posX, posY), 0.5f, 0.f});
            ECS.AddComponent(e, InputSet{SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN});
            ECS.AddComponent(e, PlayerInput());
            ECS.AddComponent(e, DebugRenderable{DebugRenderable::ShapeType::CIRCLE, glm::vec4(1,0,0,1)});        
        }   
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
        cameraSystem->Update(dt);

        renderSystem->SetScreenDimensions(window_width, window_height);
        renderSystem->Update(dt);

        SDL_GL_SwapWindow(window);
    }
}
