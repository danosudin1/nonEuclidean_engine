#version 400 core

out vec4 out_colour;		// The output colour

uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

uniform vec3 in_colour;

void main()
{
	out_colour = vec4(in_colour, 1.0f);	// Just use the colour
}
