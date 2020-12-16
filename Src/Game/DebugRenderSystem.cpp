#include "DebugRenderSystem.h"

#include "Components.h"



DebugRenderSystem::DebugRenderSystem(ECSController* ECS) : System::System(ECS)
{
    sh = std::make_unique<ShapeRenderer>();
}

void DebugRenderSystem::SetCamera(Entity entity)
{
    //please check for errors later
    this->camera = entity;
}

void DebugRenderSystem::SetScreenDimensions(int w, int h)
{
    screen_width = w;
    screen_height = h;
}

void DebugRenderSystem::Update(float dt)
{
    elapsed += dt;


    const auto& cam = ECS->GetComponent<Camera>(camera);
    sh->Begin(cam.proj, cam.view);
    
    for (auto const& entity : entities)
    {
        const auto& transform = ECS->GetComponent<Transform>(entity);
        const auto& renderable = ECS->GetComponent<DebugRenderable>(entity);

        sh->SetColor(renderable.color);

        if(renderable.type == DebugRenderable::ShapeType::CIRCLE)
        {
            sh->Circle(transform.position, transform.scale / 2.f);
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
