#version 120

attribute vec3 a_position;   
attribute vec3 a_color;   
attribute vec2 a_texcoord; 

uniform mat4 u_proj;
uniform mat4 u_view;

varying vec3 Color;
varying vec2 Texcoord;

void main()               
{              
    Color = a_color;  
    Texcoord = a_texcoord;         
    gl_Position = u_proj * u_view  * vec4(a_position.xyz, 1.0);  
}