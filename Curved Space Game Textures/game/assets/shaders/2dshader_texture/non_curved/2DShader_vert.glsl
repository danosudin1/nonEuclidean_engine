#version 430 core

#define m_pi 3.1415926535897932384626433832795

uniform mat4 u_transform;
uniform vec3 in_object_position;
uniform float in_object_rotation;

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;  

out vec2 cs_tex_coord;
out vec3 cs_position;

// function to constrain an angle to a range 0 to 2pi
float constrain_angle_rad(float angle)
{
	angle = mod(angle, 2 * m_pi);
	if (angle < 0)
		angle += 2 * m_pi;
	return angle;
}

// Function that converts polar vector into a cartesian vector
vec3 toCartesian(float R, float Theta) {
		return vec3(R * cos(Theta), R * sin(Theta), 0.0f);
}

void main()  
{
    cs_tex_coord = a_tex_coord;
	cs_position = toCartesian(a_position.x, constrain_angle_rad(a_position.y + in_object_rotation)) + toCartesian(in_object_position.x, in_object_position.y);
}
