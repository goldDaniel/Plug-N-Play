#ifndef __arm__

#include "ShapeRenderer_GL.h"


const std::size_t MAX_BUFFER_SIZE = 2048 * 3;

/**
 * @brief Resets the buffers, this will cause additional calls to
 * overwrite the data currently held.
 *
 */
void ShapeRenderer_GL::Clear()
{
    memset(&position_buffer[0], 0, MAX_BUFFER_SIZE * sizeof(float));
    memset(&color_buffer[0], 0, MAX_BUFFER_SIZE * sizeof(float));
    current_index = 0;
}

/**
 * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
 *
 */
void ShapeRenderer_GL::Flush()
{
    if (current_index == 0) return;

    assert(has_begun && "ShapeRenderer must be running to flush data to GPU");

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_index, &position_buffer[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_index, &color_buffer[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    shader->Use();
    shader->SetMat4("u_proj", proj);
    shader->SetMat4("u_view", view);

    //we divide by 3 as there are 3 values per index (x,y,z) or (r,g,b)
    glDrawArrays(GL_LINES, 0, current_index / 3);
}

/**
 * @brief Construct a new Shape Renderer object. Creates openGL buffers
 *
 */
ShapeRenderer_GL::ShapeRenderer_GL() : ShapeRenderer::ShapeRenderer()
{
    shader = std::make_unique<Shader>("Assets/Shaders/OpenGL/ShapeRendererVertex.glsl",
                                      "Assets/Shaders/OpenGL/ShapeRendererFragment.glsl");

    glGenVertexArrays(1, &vertex_array);
    
    glGenBuffers(1, &position_vbo);
    glGenBuffers(1, &color_vbo);
}

/**
 * @brief Destroy the Shape Renderer object. Deletes openGL buffers
 *
 */
ShapeRenderer_GL::~ShapeRenderer_GL()
{
    glDeleteBuffers(1, &position_vbo);
    glDeleteBuffers(1, &color_vbo);

    glDeleteVertexArrays(1, &vertex_array);
}

/**
 * @brief Adds a line shape to our buffer
 *
 * @param p0 first point for the line
 * @param p1 second point for the line
 */
void ShapeRenderer_GL::Line(const glm::vec2& p0, const glm::vec2& p1)
{
    if (current_index + 6 >= MAX_BUFFER_SIZE - 1)
    {
        Flush();
        Clear();
    }

    position_buffer[current_index + 0] = p0.x;
    position_buffer[current_index + 1] = p0.y;
    position_buffer[current_index + 2] = 0;
    color_buffer[current_index + 0] = current_color.r;
    color_buffer[current_index + 1] = current_color.g;
    color_buffer[current_index + 2] = current_color.b;

    position_buffer[current_index + 3] = p1.x;
    position_buffer[current_index + 4] = p1.y;
    position_buffer[current_index + 5] = 0;
    color_buffer[current_index + 3] = current_color.r;
    color_buffer[current_index + 4] = current_color.g;
    color_buffer[current_index + 5] = current_color.b;

    current_index += 6;
}

#endif