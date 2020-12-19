#ifndef APPLICATION_H
#define APPLICATION_H

#include <Core.h>


class Application
{
protected:
    int window_width = 1920;
    int window_height = 1080;

    SDL_Window* window;
    SDL_GLContext context;

public:
    Application()
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

        window = SDL_CreateWindow("Plug N Play",
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

    virtual ~Application()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    virtual void Run() = 0;
};

#endif