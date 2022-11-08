// Basic Texture Shader

#type vertex
#version 430

uniform bool u_polar = false;

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;  

out vec2 cs_tex_coord;
out vec3 cs_position;

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

    cs_tex_coord = a_tex_coord;
	cs_position = position;
}




#type tess_control
#version 430

layout(vertices = 3) out;

in vec2 cs_tex_coord[];
in vec3 cs_position[];

out vec2 es_tex_coord[];
out vec3 es_position[];

void main()  
{
	es_tex_coord[gl_InvocationID] = cs_tex_coord[gl_InvocationID];
	es_position[gl_InvocationID] = cs_position[gl_InvocationID];

	if (gl_InvocationID==0)
	{
		gl_TessLevelInner[0] = 3.0;

		gl_TessLevelOuter[0] = 3.0;
		gl_TessLevelOuter[1] = 3.0;
		gl_TessLevelOuter[2] = 3.0;
	}
}




#type tess_evaluation
#version 430

uniform mat4 u_view_projection;  
uniform mat4 u_transform;

layout( triangles, equal_spacing, ccw ) in;

in vec2 es_tex_coord[];
in vec3 es_position[];

out vec2 fs_tex_coord;

void main()  
{
	fs_tex_coord = es_tex_coord[0] * gl_TessCoord.x + es_tex_coord[1] * gl_TessCoord.y + es_tex_coord[2] * gl_TessCoord.z;
	vec3 position = (gl_TessCoord.x * es_position[0]) + (gl_TessCoord.y * es_position[1]) + (gl_TessCoord.z * es_position[2]);
	gl_Position = u_view_projection * u_transform * vec4(position, 1.0);
}




#type fragment
#version 430  
  
layout(location = 0) out vec4 o_color;

in vec2 fs_tex_coord;
                                        
uniform sampler2D gColorMap;                                                               
uniform vec3 gEyeWorldPos;
uniform float transparency;


uniform bool colouring_on = false;
uniform vec3 in_colour = vec3(1,0,0);
  
void main()  
{
	vec4 result;
	
	result = texture(gColorMap, fs_tex_coord.xy);

	if(colouring_on)
	{
		result = result * vec4(in_colour, transparency);
	}
	
    o_color = result;
}
