#ifndef APPLICATION_H
#define APPLICATION_H

#include <Core.h>


class Application
{
protected:
    int window_width;
    int window_height;

    SDL_Window* window;
    SDL_GLContext context;

public:
    Application(SDL_Window* window, SDL_GLContext context, int window_width, int window_height)
        : window(window), context(context), window_width(window_width), window_height(window_height) {}

    virtual ~Application() {}

    virtual void Run() = 0;

    void Resize(int w, int h)
    {
        window_width = w;
        window_height = h;
    }
};

#endif