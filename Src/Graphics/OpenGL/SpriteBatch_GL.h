#ifndef GL_ES_VERSION_2_0
#ifndef SPRITEBATCH_GL_H
#define SPRITEBATCH_GL_H


#include <Core.h>

#include <Graphics/SpriteBatch.h>

class SpriteBatch_GL : public SpriteBatch
{
private:

    static const std::size_t MAX_SPRITES = 2048;
    static const std::size_t POS_BUFFER_SIZE = MAX_SPRITES * 3 * 4;
    static const std::size_t COL_BUFFER_SIZE = MAX_SPRITES * 3 * 4;
    static const std::size_t TEX_BUFFER_SIZE = MAX_SPRITES * 2 * 4;

    static const std::size_t IDX_BUFFER_SIZE = MAX_SPRITES * 6;

    std::array<float, POS_BUFFER_SIZE> position_buffer;
    std::array<float, COL_BUFFER_SIZE> color_buffer;
    std::array<float, TEX_BUFFER_SIZE> texcoord_buffer;

    std::array<unsigned int, IDX_BUFFER_SIZE> index_buffer;
    unsigned int current_vertex_index = 0;

    std::size_t current_pos_index = 0;
    std::size_t current_col_index = 0;
    std::size_t current_tex_index = 0;
    std::size_t current_idx_index = 0;
    
    std::unique_ptr<Shader> shader;

    GLuint vertex_array;

    GLuint position_vbo;
    GLuint color_vbo;
    GLuint texcoord_vbo;

    GLuint index_vbo;

    /**
     * @brief Resets the buffers, this will cause additional calls to
     * overwrite the data currently held.
     *
     */
    void Clear() override;

    /**
     * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
     *
     */
    void Flush() override;

public:

    SpriteBatch_GL();
    ~SpriteBatch_GL() override;


    void Draw(Texture const* const tex,
        float const x, float const y,
        float const w, float const h) override;

    void Draw(Texture const* const tex,
        float const x, float const y,
        float const w, float const h,
        float rotation) override;
};

#endif
#endif