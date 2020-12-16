#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"


class Application
{
private:
    int window_width = 1920;
    int window_height = 1080;

    SDL_Window* window;
    SDL_GLContext context;

public:
    Application();

    ~Application();

    void Run();
};

#endif