
#include <Core.h>
#include <Game/GameApplication.h>
#include <Editor/EditorApplication.h>

static SDL_Window* window;
static SDL_GLContext context;
static int window_width;
static int window_height;

static std::unique_ptr<Application> app;

static int resizingEventWatcher(void* data, SDL_Event * event) 
{
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED) 
    {
        SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
        if (win == (SDL_Window*)data) 
        {
            app->Resize(event->window.data1, event->window.data2);
        }
    }
    return 0;
}

void Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    window_width = 1920; //DM.w;
    window_height = 1080; //DM.h;
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    window = SDL_CreateWindow("Plug N Play",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        window_width, window_height,
                        SDL_WINDOW_OPENGL);

    SDL_AddEventWatch(resizingEventWatcher, window);

    context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);


    //We need glad to init openGL for desktop platforms
#ifndef __arm__
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
#endif

    //IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 130");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Shutdown()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

std::unique_ptr<Application> ChooseApplication()
{
    std::unique_ptr<Application> result = nullptr;

    bool application_chosen = false;
    while (!application_chosen)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                application_chosen = true;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::Begin("Pick Application");
        {
            if (ImGui::Button("Game", ImVec2(100, 50)))
            {
                application_chosen = true;
                result = std::make_unique<GameApplication>(window, context, window_width, window_height);
            }
            ImGui::SameLine();
            if (ImGui::Button("Editor", ImVec2(100, 50)))
            {
                application_chosen = true;
                result = std::make_unique<EditorApplication>(window, context, window_width, window_height);
            }
        }
        ImGui::End();


        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)window_width, (int)window_height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    return result;
}

int main(int argc, char** argv)
{
    Init();

#ifdef __arm__
    app = std::make_unique<GameApplication>();
#else
    app = ChooseApplication();
#endif

    if (app)
    {
        app->Run();
    }
    
    
    Shutdown();

    return 0;
}

