#include <Editor/EditorApplication.h>

Bezier::Bezier<3> current_path;
static int mouseX;
static int mouseY;
static bool mouse_down = false;

static Bezier::Point* held_point = nullptr;

static void ResetCurve()
{
    current_path = Bezier::Bezier<3>({ {-3, 6},{3, 0},{-3, 0},{-3, -6} });
}


EditorApplication::EditorApplication(SDL_Window* window, SDL_GLContext context, int window_width, int window_height) 
                  : Application::Application(window, context, window_width, window_height)
{
    sh = ShapeRenderer::CreateShapeRenderer();

    ResetCurve();
}

EditorApplication::~EditorApplication() 
{

}

void EditorApplication::Run()
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
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_down = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (held_point)
                    {
                        held_point->x = glm::round(held_point->x);
                        held_point->y = glm::round(held_point->y);
                    }

                    mouse_down = false;
                    held_point = nullptr;
                }
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseY = window_height - mouseY;
            }
        }


        //INTERACTION
        const glm::mat4 proj = glm::perspective(glm::radians(67.f), (float)window_width / (float)window_height, 1.f, 100.f);
        const glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 11.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        const glm::vec4 viewport{ 0, 0, window_width, window_height };

        //You have your screen space coordinates as x and y
        glm::vec3 screenPoint1(mouseX, mouseY, 0.f);
        glm::vec3 screenPoint2(mouseX, mouseY, 100.f);

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
        glm::vec2 selection_point(point_on_plane.x, point_on_plane.y);
        


        float selection_radius = 0.2f;
        auto& control_points = current_path.getControlPoints();
        if (mouse_down)
        { 
            for (auto& control_point : control_points)
            {
                glm::vec2 p{ control_point.x, control_point.y };
                if (glm::distance(selection_point, p) < selection_radius*2)
                {
                    held_point = &control_point;
                }
            }
        }
        if (held_point)
        {
            held_point->x = selection_point.x;
            held_point->y = selection_point.y;
        }



        //RENDERING
        glViewport(0, 0, (int)window_width, (int)window_height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        sh->Begin(proj, view);

        int grid_size_x = 6;
        int grid_size_y = 8;
        for (int i = -grid_size_x; i < grid_size_x; i++)
        {
            for (int j = -grid_size_y; j < grid_size_y; j++)
            {
                sh->SetColor(glm::vec4(0, 1, 0, 1));
                glm::vec2 min(i - 0.5f, j - 0.5f);
                glm::vec2 max(i + 0.5f, j + 0.5f);
                sh->Rect(min, max);
            }
        }
        //draw blue rectangle at the origin tile
        sh->SetColor(glm::vec4(0.2f, 0.2f, 1.f, 1.f));
        glm::vec2 min(-0.5f, -0.5f);
        glm::vec2 max(0.5f, 0.5f);
        sh->Rect(min, max);

        min = glm::vec2(-3.5, -6.5);
        max = glm::vec2(+3.5, +6.5);
        sh->Rect(min, max);


        sh->SetColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
        float step = 0.05f;
        for (float t = 0.f; t < 1; t += step)
        {
            Bezier::Point p00 = current_path.valueAt(t);
            glm::vec2 p01(p00.x, p00.y);

            Bezier::Point p10 = current_path.valueAt(t + step);
            glm::vec2 p11(p10.x, p10.y);
            
            sh->Line(p01, p11);
        }

        sh->Circle(selection_point, selection_radius);
        
        
        for (const auto& point : control_points)
        {
            glm::vec2 p{ point.x, point.y };
            
            
            sh->SetColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
            if (glm::distance(selection_point, p) < selection_radius)
            {
                sh->SetColor(glm::vec4(1.f, 0.4f, 0.4f, 1.f));
            }
            sh->Circle(p, selection_radius, 16);
        }

        sh->End();


        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Path"))
                {
                    ResetCurve();
                }
                if (ImGui::MenuItem("Save Path"))
                {

                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();



        ImGui::Render();        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}
