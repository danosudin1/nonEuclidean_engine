#version 430 core

uniform vec3 in_object_position;
uniform int num_segments;

layout(vertices = 3) out;

in vec2 cs_tex_coord[];
in vec3 cs_position[];

out vec2 es_tex_coord[];
out vec3 es_position[];

// Function that converts polar vector into a cartesian vector
vec3 toCartesian(float R, float Theta) {
		return vec3(R * cos(Theta), R * sin(Theta), 0.0f);
}

void main()  
{
	if (!(gl_InvocationID==2))
	{
		es_tex_coord[gl_InvocationID] = cs_tex_coord[gl_InvocationID];
		es_position[gl_InvocationID] = cs_position[gl_InvocationID];
	}
	else
	{
		es_tex_coord[gl_InvocationID] = vec2(0.5f, 0.5f);
		es_position[gl_InvocationID] = toCartesian(in_object_position.x, in_object_position.y);

		gl_TessLevelInner[0] = num_segments;

		gl_TessLevelOuter[0] = num_segments;
		gl_TessLevelOuter[1] = num_segments;
		gl_TessLevelOuter[2] = num_segments;
	}
}
