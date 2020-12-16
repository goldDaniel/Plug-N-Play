#version 120

attribute vec3 a_position;   
attribute vec3 a_color;   

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

varying vec3 Color;

void main()               
{              
    Color = a_color;           
    gl_Position = u_proj * u_view * u_model * vec4(a_position.xyz, 1.0);  
}