#ifndef GL_ES_VERSION_2_0
#ifndef SHAPE_RENDERER_GL_H
#define SHAPE_RENDERER_GL_H

#include <Core.h>

#include <Graphics/ShapeRenderer.h>
#include <Graphics/Shader.h>


/**
 * @brief Batch draw shapes using an immediate-mode interface
 *
 */
class ShapeRenderer_GL : public ShapeRenderer
{
private:

    /**
     * @brief The number of elements in the data buffers.
     * This can be adjusted based on memory requirements.
     */
    static const std::size_t MAX_BUFFER_SIZE = 2048 * 3;

    /**
     * @brief The location we are current at in our buffers
     *
     */
    std::size_t current_index = 0;
    //xyz
    std::array<float, MAX_BUFFER_SIZE> position_buffer{0};
    //rgb
    std::array<float, MAX_BUFFER_SIZE> color_buffer{0};

    std::unique_ptr<Shader> shader;

    GLuint vertex_array;

    GLuint position_vbo;
    GLuint color_vbo;

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

    /**
     * @brief Construct a new Shape Renderer object. Creates openGL buffers
     *
     */
    ShapeRenderer_GL();

    /**
     * @brief Destroy the Shape Renderer object. Deletes openGL buffers
     *
     */
    ~ShapeRenderer_GL() override;

    /**
     * @brief Adds a line shape to our buffer
     *
     * @param p0 first point for the line
     * @param p1 second point for the line
     */
    void Line(const glm::vec2& p0, const glm::vec2& p1) override;
};

#endif
#endif