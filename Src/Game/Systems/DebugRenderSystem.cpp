#include "DebugRenderSystem.h"



DebugRenderSystem::DebugRenderSystem(ECSController* ECS) : System::System(ECS)
{
    sh = ShapeRenderer::CreateShapeRenderer();
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
    
    int grid_size = 8;
    for(int i = -grid_size; i < grid_size; i++)
    {
        for(int j = -grid_size; j < grid_size; j++)
        {
            sh->SetColor(glm::vec4(0,1,0,1));
            glm::vec2 min(i - 0.5f, j - 0.5f);
            glm::vec2 max(i + 0.5f, j + 0.5f);
            sh->Rect(min, max);
        }
    }
    //draw blue rectangle at the origin tile
    sh->SetColor(glm::vec4(0.2f, 0.2f, 1.f, 1.f));
    glm::vec2 min(-0.5f, -0.5f);
    glm::vec2 max( 0.5f,  0.5f);
    sh->Rect(min, max);

    sh->SetColor(glm::vec4(1.f, 0.2f, 0.2f, 1.f));
    min = glm::vec2(-3.5, -6.5);
    max = glm::vec2(+3.5, +6.5);
    sh->Rect(min, max);

    for (auto const& entity : entities)
    {
        const auto& transform = ECS->GetComponent<Transform>(entity);
        const auto& renderable = ECS->GetComponent<DebugRenderable>(entity);

        sh->SetColor(renderable.color);

        if(renderable.type == DebugRenderable::ShapeType::CIRCLE)
        {
            sh->Ellipse(transform.position, 
                        transform.scale.x / 2.f * renderable.scale, 
                        transform.scale.y / 2.f * renderable.scale);
        }
        if(renderable.type == DebugRenderable::ShapeType::RECT)
        {
            glm::vec2 p0 = transform.position;
            p0.x -= transform.scale.x / 2.f * renderable.scale;
            p0.y -= transform.scale.y / 2.f * renderable.scale;

            glm::vec2 p1 = transform.position;
            p1.x += transform.scale.x / 2.f * renderable.scale;
            p1.y += transform.scale.y / 2.f * renderable.scale;

            sh->Rect(p0, p1);
        }
    }
        
    sh->End();
}
