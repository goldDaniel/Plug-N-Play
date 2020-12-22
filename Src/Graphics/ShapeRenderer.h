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
protected:

    bool has_begun = false;


    glm::vec4 current_color = glm::vec4(1, 1, 1, 1);

    glm::mat4 proj = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);


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

    static std::unique_ptr<ShapeRenderer> CreateShapeRenderer();

    /**
     * @brief Destroy the Shape Renderer object. Deletes openGL buffers
     *
     */
    virtual ~ShapeRenderer() {}

    /**
     * @brief Sets the color for all following shapes.
     *
     * @param color The color the following shapes will be. Ignores alpha channel
     */
    void SetColor(const glm::vec4& color)
    {
        this->current_color = color;
    }

    /**
     * @brief Creates a new set of shapes to be batched & sent to the GPU.
     * 
     * @param proj Projection matrix for this call
     * @param view View matrix for this call
     */
    void Begin(const glm::mat4& proj, const glm::mat4& view)
    {
        assert(!has_begun && "ShapeRenderer->End() must be called before ShapeRenderer->Begin()");

        Clear();
        this->proj = proj;
        this->view = view;

        has_begun = true;
    }

    const glm::mat4& GetProjectionMatrix()
    {
        return proj;
    }

    const glm::mat4& GetViewMatrix()
    {
        return view;
    }

    /**
     * @brief Adds a line shape to our buffer
     * 
     * @param p0 first point for the line
     * @param p1 second point for the line
     */
    virtual void Line(const glm::vec2& p0, const glm::vec2& p1) = 0;

    /**
     * @brief Adds a circle shape consisting of line segments
     * 
     * @param position the center of the circle
     * @param radius the radius of the circle
     * @param segments how many segments make up the circle
     */
    void Circle(const glm::vec2& position, float radius, int segments = 32)
    {
        Ellipse(position, radius, radius, segments);
    }


    /**
     * @brief Adds an ellipse shape consisting of line segments
     * 
     * @param position the center of the circle
     * @param rX the radius along the X-axis
     * @param rY the radius along the Y-axis
     * @param segments how many segments make up the ellipse
     */
    void Ellipse(const glm::vec2& position, float rX, float rY, int segments = 32)
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
    void Rect(const glm::vec2& min, const glm::vec2& max)
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
    void End()
    {
        assert(has_begun && "ShapeRenderer->Begin() must be called before ShapeRenderer->End()");

        Flush();
        Clear();

        has_begun = false;
    }
};

#endif