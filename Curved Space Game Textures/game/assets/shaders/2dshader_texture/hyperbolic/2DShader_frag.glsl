#version 430 core
  
layout(location = 0) out vec4 o_color;

in vec2 fs_tex_coord;
                                        
uniform sampler2D gColorMap;
  
void main()  
{
	vec4 tex_color = texture(gColorMap, fs_tex_coord.xy);

	if(tex_color.a < 0.1)
        discard;

    o_color = tex_color;

}
