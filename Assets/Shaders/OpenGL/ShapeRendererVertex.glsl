#version 330 core

layout (location = 0) in vec3 a_position;   
layout (location = 1) in vec3 a_color;   

uniform mat4 u_proj;
uniform mat4 u_view;

out vec3 Color;

void main()               
{              
    Color = a_color;           
    gl_Position = u_proj * u_view * vec4(a_position.xyz, 1.0);  
}