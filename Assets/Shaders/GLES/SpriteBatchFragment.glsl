varying vec3 Color;
varying vec2 Texcoord;

uniform sampler2D u_texture;

void main()                                 
{                  
    vec4 texture_color = texture2D(u_texture, Texcoord);
    
    gl_FragColor = vec4(texture_color.rgb * Color, texture_color.a);
}                                           