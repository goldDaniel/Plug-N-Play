#include "SpriteBatch.h"

#ifdef __arm__
    #include <Graphics/GLES/SpriteBatch_ES.h>
    #define SHAPERENDERER_IMPL SpriteBatch_ES
#else
    #include <Graphics/OpenGL/SpriteBatch_GL.h>
    #define SHAPERENDERER_IMPL SpriteBatch_GL
#endif

std::unique_ptr<SpriteBatch> SpriteBatch::CreateSpriteBatch()
{
    return std::make_unique<SHAPERENDERER_IMPL>();
}