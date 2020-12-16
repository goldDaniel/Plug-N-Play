#include "RenderSystem.h"

#include "Components.h"



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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& cam = ECS->GetComponent<Camera>(camera);
    cam.proj = glm::perspective(glm::radians(67.f), (float)screen_width/(float)screen_height, 1.f, 100.f);;
    s->Begin(cam.proj, cam.view);
    
    for (auto const& entity : entities)
    {
        const auto& transform = ECS->GetComponent<Transform>(entity);
        const auto& renderable = ECS->GetComponent<Renderable>(entity);

        s->SetColor(renderable.color);
        s->Draw(renderable.texture, 
                transform.position.x, transform.position.y, 
                transform.scale, transform.scale);
    }

    s->End();
}
