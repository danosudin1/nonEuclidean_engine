#version 400 core

out vec4 out_colour;

uniform vec3 in_colour;

void main()
{
	
	out_colour = vec4(in_colour, 1.0f);	// Just use the colour
	
}
