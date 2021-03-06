#version 120

attribute vec3 a_position;   
attribute vec3 a_color;   

uniform mat4 u_proj;
uniform mat4 u_view;

varying vec3 Color;

void main()               
{              
    Color = a_color;           
    gl_Position = u_proj * u_view * vec4(a_position.xyz, 1.0);  
}