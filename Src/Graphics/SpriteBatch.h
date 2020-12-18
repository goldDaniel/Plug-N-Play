#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H


#include <Core.h>

#include "Shader.h"
#include "Texture.h"

class SpriteBatch
{
protected:

    bool has_begun = false;

    glm::mat4 proj = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);

    Texture const* current_texture = nullptr;

    glm::vec4 current_color = glm::vec4(1, 1, 1, 1);

    
    /**
     * @brief Resets the buffers, this will cause additional calls to 
     * overwrite the data currently held.
     * 
     */
    virtual void Clear() = 0;

    /**
     * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
     * 
     */
    virtual void Flush() = 0;

public:
   
    static std::unique_ptr<SpriteBatch> CreateSpriteBatch();

    virtual ~SpriteBatch() {}

    /**
     * @brief Sets the color for all following shapes.
     * 
     * @param color The color the following shapes will be. Ignores alpha channel
     */
    void SetColor(const glm::vec4& color)
    {
        this->current_color = color;
    }

    void Begin(glm::mat4 proj, glm::mat4 view)
    {
        assert(!has_begun && "Spritebatch.end must be called before Spritebatch.begin");

        Clear();
        this->proj = proj;
        this->view = view;

        has_begun = true;
    }

    virtual void Draw(Texture const * const tex, float const x, float const y, float const w, float const h) = 0;

    void End()
    {
        assert(has_begun && "Spritebatch.end must be called before Spritebatch.begin");

        Flush();
        Clear();

        has_begun = false;
    }
};

#endif