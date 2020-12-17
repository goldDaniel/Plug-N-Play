#include "RenderSystem.h"

RenderSystem::RenderSystem(ECSController* ECS) : System::System(ECS)
{
    s = std::make_unique<SpriteBatch>();
}

void RenderSystem::SetCamera(Entity entity)
{
    this->camera = entity;
}

void RenderSystem::SetScreenDimensions(int w, int h)
{
    screen_width = w;
    screen_height = h;
}

void RenderSystem::Update(float dt)
{
    elapsed += dt;

    const auto& cam = ECS->GetComponent<Camera>(camera);
    s->Begin(cam.proj, cam.view);
    
    for (auto const& entity : entities)
    {
        const auto& transform = ECS->GetComponent<Transform>(entity);
        const auto& renderable = ECS->GetComponent<Renderable>(entity);

        s->SetColor(renderable.color);
        s->Draw(    renderable.texture, 
                    transform.position.x, transform.position.y, 
                    transform.scale.x,  transform.scale.y);
    }

    s->End();
}
