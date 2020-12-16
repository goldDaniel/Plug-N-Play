#include "DebugRenderSystem.h"

#include "Components.h"



DebugRenderSystem::DebugRenderSystem(ECSController* ECS) : System::System(ECS)
{
    sh = std::make_unique<ShapeRenderer>();

    camera = ECS->CreateEntity();
    Camera cam_comp;
    cam_comp.proj = glm::mat4(1.f);
    cam_comp.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    ECS->AddComponent(camera, cam_comp);
}

void DebugRenderSystem::SetScreenDimensions(int w, int h)
{
    screen_width = w;
    screen_height = h;
    ECS->GetComponent<Camera>(camera).proj = glm::perspective(glm::radians(67.f), (float)screen_width/(float)screen_height, 1.f, 100.f);;
}

void DebugRenderSystem::Update(float dt)
{
    elapsed += dt;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& cam = ECS->GetComponent<Camera>(camera);
    sh->Begin(cam.proj, cam.view);
    
    for (auto const& entity : entities)
    {
        const auto& transform = ECS->GetComponent<Transform>(entity);
        const auto& renderable = ECS->GetComponent<DebugRenderable>(entity);

        sh->SetColor(renderable.color);

        if(renderable.type == DebugRenderable::ShapeType::CIRCLE)
        {
            sh->Circle(transform.position, transform.scale);
        }
        if(renderable.type == DebugRenderable::ShapeType::RECT)
        {
            glm::vec2 p0 = transform.position;
            p0.x -= transform.scale / 2.f;
            p0.y -= transform.scale / 2.f;

            glm::vec2 p1 = transform.position;
            p1.x += transform.scale / 2.f;
            p1.y += transform.scale / 2.f;

            sh->Rect(p0, p1);
        }
    }

    sh->End();
}
