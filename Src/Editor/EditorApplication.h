#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

#include <Core.h>
#include <Application.h>

class EditorApplication : public Application
{
public:
    EditorApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height)
        : Application::Application(window, context, window_width, window_height) {}

    virtual ~EditorApplication() override
    {

    }

    virtual void Run() override
    {

    }
};

#endif