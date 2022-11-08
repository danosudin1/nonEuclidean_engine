// Basic Texture Shader

#type vertex
#version 430  

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coord;  

uniform mat4 u_view_projection;  
uniform mat4 u_transform;
uniform bool u_polar = false;

out vec2 v_tex_coord;
out vec3 v_position;
out vec3 v_normal;
out vec4 v_pos;

// Function that converts polar vector into a cartesian vector
vec3 toCartesian(float R, float Theta) {
		return vec3(R * cos(Theta), R * sin(Theta), 0.0f);
}

void main()  
{
	vec3 position;
	if(u_polar) {
		position = toCartesian(a_position.x, a_position.y);
	}
	else {
		position = a_position;
	}

    v_tex_coord = a_tex_coord;
	v_position = vec3(u_transform * vec4(position, 1.0));
    v_normal = mat3(transpose(inverse(u_transform))) * a_normal;
	v_pos = u_view_projection * u_transform * vec4(position, 1.0); 
    gl_Position = v_pos;
}  

#type fragment
#version 430  
  
layout(location = 0) out vec4 o_color;

in vec2 v_tex_coord;
in vec3 v_position;
in vec3 v_normal;                                                              

struct VSOutput
{
    vec2 TexCoord;
    vec3 Normal;                                                                   
    vec3 WorldPos;                                                                 
};                                           
uniform sampler2D gColorMap;                                                               
uniform vec3 gEyeWorldPos;
uniform float transparency;
in vec4 v_pos;

uniform bool colouring_on = false;
uniform vec3 in_colour = vec3(1,0,0);
  
void main()  
{
	VSOutput In;
    In.TexCoord = v_tex_coord;
    In.Normal   = normalize(v_normal);
    In.WorldPos = v_position;

	vec4 result;
	
	result = texture(gColorMap, In.TexCoord.xy);

	if(colouring_on)
	{
		result = result * vec4(in_colour, transparency);
	}
	
    o_color = result;
}
