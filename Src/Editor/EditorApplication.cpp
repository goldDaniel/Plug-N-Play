#include <Editor/EditorApplication.h>

#include <imgui/ImGuiFileBrowser.h>

#include "CurveEditor.h"
#include "StageEditor.h"


static glm::vec2 ProjectToXY0Plane(glm::vec2 mouse_pos, 
                                   const glm::mat4& view, 
                                   const glm::mat4& proj, 
                                   float window_width, 
                                   float window_height)
{
    

    const glm::vec4 viewport{ 0, 0, window_width, window_height };

    //You have your screen space coordinates as x and y
    glm::vec3 screenPoint1(mouse_pos.x, mouse_pos.y, 0.f);
    glm::vec3 screenPoint2(mouse_pos.x, mouse_pos.y, 100.f);

    //Unproject both these points
    glm::vec3 modelPoint1 = glm::unProject(screenPoint1, view, proj, viewport);
    glm::vec3 modelPoint2 = glm::unProject(screenPoint2, view, proj, viewport);

    //normalOfPlane is the normal of the plane. If it's a xy plane then the normal is vec3(0,0,1)
    //P0 is a point on the plane

    glm::vec3 plane_normal(0, 0, 1);
    //L is the direction of your ray
    //L0 is a point on the ray
    glm::vec3 L = glm::normalize(modelPoint2 - modelPoint1);
    glm::vec3 L0 = modelPoint1;

    //Solve for d where dot((d * L + L0 - P0), n) = 0
    float d = glm::dot(-L0, plane_normal) / glm::dot(L, plane_normal);

    //Use d to get back to point on plane
    glm::vec3 point_on_plane = d * L + L0;
    
    
    return glm::vec2(point_on_plane.x, point_on_plane.y);
}

EditorApplication::EditorApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height) 
                  : Application::Application(window, context, window_width, window_height)
{
    sh = ShapeRenderer::CreateShapeRenderer();
    editor = std::make_unique<CurveEditor>();
}

EditorApplication::~EditorApplication() {}

void EditorApplication::Run()
{
    bool running = true;

    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseY = window_height - mouseY;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            
            if (event.type == SDL_QUIT) running = false;
            
            if (event.type == SDL_MOUSEBUTTONDOWN)
                if (event.button.button == SDL_BUTTON_LEFT)
                    editor->OnMouseButtonDown();

            if (event.type == SDL_MOUSEBUTTONUP)
                if (event.button.button == SDL_BUTTON_LEFT)
                    editor->OnMouseButtonUp();
            
            
            if (event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseY = window_height - mouseY;
            }
        }

        const glm::mat4 proj = glm::perspective(glm::radians(67.f), (float)window_width / (float)window_height, 1.f, 100.f);
        const glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 11.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        //INTERACTION
        glm::vec2 selection_point = ProjectToXY0Plane({ mouseX, mouseY }, view, proj, window_width, window_height);
        
        editor->Update(selection_point);


        //RENDERING
        glViewport(0, 0, (int)window_width, (int)window_height);
        glClearColor(0.01f, 0.01f, 0.05f, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        sh->Begin(proj, view);

        int grid_size_x = 6;
        int grid_size_y = 8;
        sh->SetColor(glm::vec4(0.1f, 0.4f, 0.1f, 1));
        for (int i = -grid_size_x; i <= grid_size_x; i++)
        {
            for (int j = -grid_size_y; j < grid_size_y; j++)
            {
                glm::vec2 min(i - 0.5f, j - 0.5f);
                glm::vec2 max(i + 0.5f, j + 0.5f);
                sh->Rect(min, max);
            }
        }
        //draw blue rectangle at the origin tile
        sh->SetColor(glm::vec4(0.5f, 0.5f, 0.9f, 1.f));
        glm::vec2 min(-0.5f, -0.5f);
        glm::vec2 max(0.5f, 0.5f);
        sh->Rect(min, max);

        //draws border for gameplay window
        sh->SetColor(glm::vec4(0.6f, 0.2f, 0.2f, 1.f));
        min = glm::vec2(-3.5, -6.5);
        max = glm::vec2(+3.5, +6.5);
        sh->Rect(min, max);

        editor->Render(*sh);

        sh->End();


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Quit"))
                {
                    running = false;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editor"))
            {
                if (ImGui::MenuItem("Curve Editor"))
                {
                    editor = std::make_unique<CurveEditor>();
                }
                if (ImGui::MenuItem("Stage Editor"))
                {
                    editor = std::make_unique<StageEditor>();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        editor->OnGUIRender();



        ImGui::Render();        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}
