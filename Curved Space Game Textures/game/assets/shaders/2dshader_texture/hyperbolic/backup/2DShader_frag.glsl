#version 430 core
  
layout(location = 0) out vec4 o_color;

in vec2 fs_tex_coord;
                                        
uniform sampler2D gColorMap;
  
void main()  
{
    o_color = texture(gColorMap, fs_tex_coord.xy);
}
