varying vec3 Color;
varying vec2 Texcoord;

uniform sampler2D u_texture;

void main()                                 
{                  
    vec4 final_color = texture2D(u_texture, Texcoord);                        
    final_color.rgb *= Color; 

    gl_FragColor = final_color;
}                                           