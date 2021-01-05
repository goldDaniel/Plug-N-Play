#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

#include <Core.h>
#include <Application.h>
#include <Graphics/ShapeRenderer.h>

class Editor
{
public:
    virtual void OnGUIRender() = 0;

    virtual void Render(ShapeRenderer& sh) = 0;

    virtual void OnMouseButtonDown() = 0;
    virtual void OnMouseButtonUp() = 0;

    virtual void Update(glm::vec2 mouse_world_pos) = 0;
};

class EditorApplication : public Application
{
private:
    std::unique_ptr<ShapeRenderer> sh;

    std::shared_ptr<Editor> stage_editor;
    std::shared_ptr<Editor> curve_editor;

    std::shared_ptr<Editor> current_editor;

    bool show_console = false;

public:
    EditorApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height);
    
    virtual ~EditorApplication() override;

    virtual void Run() override;
};

#endif