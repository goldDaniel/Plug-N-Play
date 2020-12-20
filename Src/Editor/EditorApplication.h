#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

#include <Core.h>
#include <Application.h>
#include <Graphics/ShapeRenderer.h>

class EditorApplication : public Application
{
private:
    std::unique_ptr<ShapeRenderer> sh;

public:
    EditorApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height) : Application::Application(window, context, window_width, window_height) 
    {
        sh = ShapeRenderer::CreateShapeRenderer();
    }

    virtual ~EditorApplication() override
    {

    }

    virtual void Run() override
    {
        bool running = true;


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

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();







            // Rendering
            ImGui::Render();
            glViewport(0, 0, (int)window_width, (int)window_height);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }  
    }
};

#endif