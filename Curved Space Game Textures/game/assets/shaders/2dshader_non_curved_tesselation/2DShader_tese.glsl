#version 430 core

uniform mat4 u_view_projection;

layout( triangles, equal_spacing, ccw ) in;

in vec2 es_tex_coord[];
in vec3 es_position[];

out vec2 fs_tex_coord;

void main()  
{
	fs_tex_coord = es_tex_coord[0] * gl_TessCoord.x + es_tex_coord[1] * gl_TessCoord.y + es_tex_coord[2] * gl_TessCoord.z;
	vec3 position = (gl_TessCoord.x * es_position[0]) + (gl_TessCoord.y * es_position[1]) + (gl_TessCoord.z * es_position[2]);
	gl_Position = u_view_projection * vec4(position, 1.0);
}
