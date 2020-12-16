#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include <Core.h>

#include "Shader.h"


/**
 * @brief Batch draw shapes using an immediate-mode interface 
 *
 */
class ShapeRenderer
{
private:

    /**
     * @brief The number of elements in the data buffers.
     * This can be adjusted based on memory requirements.
     */
    static const std::size_t MAX_BUFFER_SIZE = 2048 * 3;

    /**
     * @brief If the shapeRenderer is currently running
     * 
     */
    bool has_begun = false;

    /**
     * @brief The location we are current at in our buffers
     * 
     */
    std::size_t current_index = 0;
    //xyz
    std::array<float, MAX_BUFFER_SIZE> position_buffer;
    //rgb
    std::array<float, MAX_BUFFER_SIZE> color_buffer;

    glm::vec3 current_color = glm::vec3(1,1,1);

    glm::mat4 proj = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);

    std::unique_ptr<Shader> shader;

    GLuint position_vbo;
    GLuint color_vbo;

    /**
     * @brief Resets the buffers, this will cause additional calls to 
     * overwrite the data currently held.
     * 
     */
    void Clear();

    /**
     * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
     * 
     */
    void Flush();

public:

    /**
     * @brief Construct a new Shape Renderer object. Creates openGL buffers
     * 
     */
    ShapeRenderer();

    /**
     * @brief Destroy the Shape Renderer object. Deletes openGL buffers
     * 
     */
    ~ShapeRenderer();

    /**
     * @brief Sets the color for all following shapes.
     * 
     * @param color The color the following shapes will be. Ignores alpha channel
     */
    void SetColor(const glm::vec4& color);

    /**
     * @brief Creates a new set of shapes to be batched & sent to the GPU.
     * 
     * @param proj Projection matrix for this call
     * @param view View matrix for this call
     */
    void Begin(const glm::mat4& proj, const glm::mat4& view);

    /**
     * @brief Adds a line shape to our buffer
     * 
     * @param p0 first point for the line
     * @param p1 second point for the line
     */
    void Line(const glm::vec2& p0, const glm::vec2& p1);

    /**
     * @brief Adds a circle shape consisting of line segments
     * 
     * @param position the center of the circle
     * @param radius the radius of the circle
     * @param segments how many segments make up the circle
     */
    void Circle(const glm::vec2& position, float radius, int segments = 32);

    /**
     * @brief Adds a circle shape consisting of line segments
     * 
     * @param position the center of the circle
     * @param rX the radius along the X-axis
     * @param rY the radius along the Y-axis
     * @param segments how many segments make up the circle
     */
    void Ellipse(const glm::vec2& position, float rX, float rY, int segments = 32);

    /**
     * @brief Adds a rectangle shape to the buffer
     * 
     * @param min bottom-left corner of the rectangle
     * @param max top-right corner of the rectangle
     */
    void Rect(const glm::vec2& min, const glm::vec2& max);

    /**
     * @brief Ends the batch call. This will send Data to the GPU if it has not been done already
     * 
     */
    void End();
};

#endif