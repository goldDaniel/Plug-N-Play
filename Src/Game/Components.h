#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include <glm/glm.hpp>
#include <Graphics/Texture.h>

#include <bezier.h>

struct Camera
{
    glm::mat4 proj = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);;
};

struct Transform
{ 
    glm::vec2 position = glm::vec2(0,0);
    glm::vec2 scale = glm::vec2(1,1);
    float rotation = 0;
};

struct BezierPath
{   
    Bezier::Bezier<2> curve;
    
    //how much per second we increase time by
    float speed = 0;

    //the accumulator that will go between 0.f - 1.f
    float time = 0;
};

struct Velocity
{
    glm::vec2 velocity{0,0};
};

struct Collider
{
    std::uint16_t collides_with = 0;
    float radius = 0;
};

struct InputSet
{
    int key_left = 0;
    int key_right = 0;
    int key_up = 0;
    int key_down = 0;

    int key_shoot = 0;
};

struct Bullet
{
};

struct PlayerInput
{
    bool key_left = false;
    bool key_right = false;
    bool key_up = false;
    bool key_down = false;

    bool key_shoot = false;
};

struct Weapon
{
    float cooldown = 0;
    float cooldown_timer = 0;
};

struct Renderable
{
    glm::vec4 color;
    Texture* texture;
};

struct DebugRenderable
{
    enum ShapeType
    {
        RECT,
        CIRCLE,
    };

    ShapeType type = ShapeType::CIRCLE;
    glm::vec4 color = glm::vec4(1,1,1,1);
};

#endif
