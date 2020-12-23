#ifndef DEBUG_RENDER_SYSTEM_H
#define DEBUG_RENDER_SYSTEM_H

#include <Core.h>
#include <Graphics/ShapeRenderer.h>



class ECSController;


class DebugRenderSystem : public System
{

private:
    std::unique_ptr<ShapeRenderer> sh;

    Entity camera = 0;

    int screen_width;
    int screen_height;

    float elapsed;

public:

    DebugRenderSystem(ECSController * const ECS);

    void SetCamera(Entity entity);

    void SetScreenDimensions(int w, int h);
    void Update(float dt);
};

#endif