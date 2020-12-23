#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <Core.h>
#include <Graphics/SpriteBatch.h>

class RenderSystem : public System
{
private:
    std::unique_ptr<SpriteBatch> s;

    Entity camera = 0;

    int screen_width;
    int screen_height;

    float elapsed;

public:

    RenderSystem(ECSController * const ECS);

    void SetCamera(Entity entity);

    void SetScreenDimensions(int w, int h);
    void Update(float dt);
};

#endif 