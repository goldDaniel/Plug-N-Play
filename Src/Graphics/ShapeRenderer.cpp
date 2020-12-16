#include "ShapeRenderer.h"


/**
 * @brief Resets the buffers, this will cause additional calls to 
 * overwrite the data currently held.
 * 
 */
void ShapeRenderer::Clear()
{
    memset(&position_buffer[0], 0, MAX_BUFFER_SIZE * sizeof(float));
    memset(&color_buffer[0], 0, MAX_BUFFER_SIZE * sizeof(float));
    current_index = 0;
}

/**
 * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
 * 
 */
void ShapeRenderer::Flush()
{
    if(current_index == 0) return; 

    assert(has_begun && "ShapeRenderer must be running to flush data to GPU");

    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_index, &position_buffer[0], GL_DYNAMIC_DRAW);
    GLint pos_attrib = glGetAttribLocation(shader->ID, "a_position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_index, &color_buffer[0], GL_DYNAMIC_DRAW);
    GLint col_attrib = glGetAttribLocation(shader->ID, "a_color");
    glEnableVertexAttribArray(col_attrib);
    glVertexAttribPointer(col_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
ShapeRenderer::ShapeRenderer()
{
    shader = std::make_unique<Shader>("Assets/Shaders/ShapeRendererVertex.glsl", "Assets/Shaders/ShapeRendererFragment.glsl");

    glGenBuffers(1, &position_vbo);
    glGenBuffers(1, &color_vbo);
}

/**
 * @brief Destroy the Shape Renderer object. Deletes openGL buffers
 * 
 */
ShapeRenderer::~ShapeRenderer()
{
    glDeleteBuffers(1, &position_vbo);
    glDeleteBuffers(1, &color_vbo);
}

/**
 * @brief Sets the color for all following shapes.
 * 
 * @param color The color the following shapes will be. Ignores alpha channel
 */
void ShapeRenderer::SetColor(const glm::vec4& color)
{
    this->current_color = glm::vec3(color.r, color.g, color.b);
}

/**
 * @brief Creates a new set of shapes to be batched & sent to the GPU.
 * 
 * @param proj Projection matrix for this call
 * @param view View matrix for this call
 */
void ShapeRenderer::Begin(const glm::mat4& proj, const glm::mat4& view)
{
    assert(!has_begun && "ShapeRenderer->End() must be called before ShapeRenderer->Begin()");

    Clear();
    this->proj = proj;
    this->view = view;

    has_begun = true;
}

/**
 * @brief Adds a line shape to our buffer
 * 
 * @param p0 first point for the line
 * @param p1 second point for the line
 */
void ShapeRenderer::Line(const glm::vec2& p0, const glm::vec2& p1)
{
    if(current_index + 6 >= MAX_BUFFER_SIZE - 1)
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

/**
 * @brief Adds a circle shape consisting of line segments
 * 
 * @param position the center of the circle
 * @param radius the radius of the circle
 * @param segments how many segments make up the circle
 */
void ShapeRenderer::Circle(const glm::vec2& position, float radius, int segments)
{
    for(int i = 0; i < segments; i++)
    {
        float angle0 = (float)i / (float)segments * glm::pi<float>() * 2;
        float angle1 = (float)(i + 1) / (float)segments * glm::pi<float>() * 2;
        
        glm::vec2 p0 = position + glm::vec2(glm::cos(angle0), glm::sin(angle0)) * radius;
        glm::vec2 p1 = position + glm::vec2(glm::cos(angle1), glm::sin(angle1)) * radius;

        Line(p0, p1);
    }
}

/**
 * @brief Adds a circle shape consisting of line segments
 * 
 * @param position the center of the circle
 * @param rX the radius along the X-axis
 * @param rY the radius along the Y-axis
 * @param segments how many segments make up the circle
 */
void ShapeRenderer::Ellipse(const glm::vec2& position, float rX, float rY, int segments)
{
    for(int i = 0; i < segments; i++)
    {
        float angle0 = (float)i / (float)segments * glm::pi<float>() * 2;
        float angle1 = (float)(i + 1) / (float)segments * glm::pi<float>() * 2;
        
        glm::vec2 p0 = position + glm::vec2(glm::cos(angle0) * rX, glm::sin(angle0) * rY);
        glm::vec2 p1 = position + glm::vec2(glm::cos(angle1) * rX, glm::sin(angle1) * rY);

        Line(p0, p1);
    }
}

/**
 * @brief Adds a rectangle shape to the buffer
 * 
 * @param min bottom-left corner of the rectangle
 * @param max top-right corner of the rectangle
 */
void ShapeRenderer::Rect(const glm::vec2& min, const glm::vec2& max)
{
    glm::vec2 p0(min.x, min.y);
    glm::vec2 p1(max.x, min.y);
    glm::vec2 p2(max.x, max.y);
    glm::vec2 p3(min.x, max.y);

    Line(p0, p1);
    Line(p1, p2);
    Line(p2, p3);
    Line(p3, p0);
}

/**
 * @brief Ends the batch call. This will send Data to the GPU if it has not been done already
 * 
 */
void ShapeRenderer::End()
{
    assert(has_begun && "ShapeRenderer->Begin() must be called before ShapeRenderer->End()");
    
    Flush();
    Clear();

    has_begun = false;
}
