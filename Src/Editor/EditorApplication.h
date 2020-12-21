#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

#include <Core.h>
#include <Application.h>
#include <Graphics/ShapeRenderer.h>

#include "CurveEditor.h"

class EditorApplication : public Application
{
private:
    std::unique_ptr<ShapeRenderer> sh;

    std::unique_ptr<CurveEditor> curve_editor;

public:
    EditorApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height);
    
    virtual ~EditorApplication() override;

    virtual void Run() override;
};

#endif