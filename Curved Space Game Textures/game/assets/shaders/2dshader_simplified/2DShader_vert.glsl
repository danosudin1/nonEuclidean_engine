#version 400 core

// Matrices
uniform mat4 u_transform;
uniform mat4 u_view_projection;

uniform vec3 u_colour_in;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 a_position;

out vec3 colour_out;

// Function that converts polar vector into a cartesian vector
vec3 to_cartesian(float r, float theta) {
		return vec3(r * cos(theta), r * sin(theta), 0.0f);
}

// This is the entry point into the vertex shader
void main()
{	
	// Transform the vertex spatial position using 
	gl_Position = u_view_projection * u_transform * vec4(to_cartesian(a_position.x, a_position.y), 1.0f);
		
	// Pass the colour on to the fragment shader
	colour_out = u_colour_in;
} 

