#version 330 core

layout (location = 0) in vec3 a_position;   
layout (location = 1) in vec3 a_color;   
layout (location = 2) in vec2 a_texcoord; 

uniform mat4 u_proj;
uniform mat4 u_view;

out vec3 Color;
out vec2 Texcoord;

void main()               
{              
    Color = a_color;  
    Texcoord = a_texcoord;         
    gl_Position = u_proj * u_view  * vec4(a_position.xyz, 1.0);  
}