#include "ShapeRenderer.h"

#ifdef __arm__
    #include <Graphics/GLES/ShapeRenderer_ES.h>
    #define SHAPERENDERER_IMPL ShapeRenderer_ES
#else
    #include <Graphics/OpenGL/ShapeRenderer_GL.h>
    #define SHAPERENDERER_IMPL ShapeRenderer_GL
#endif

std::unique_ptr<ShapeRenderer> ShapeRenderer::CreateShapeRenderer()
{
    return std::make_unique<SHAPERENDERER_IMPL>(); 
}