#version 400 core

#define m_pi 3.1415926535897932384626433832795

layout( isolines ) in;

uniform int num_segments;
uniform float in_k;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

in float d[];
in float cos_alpha[];

in float cosh_r_out[];
in float sinh_r_out[];

in int final_direction[];

in float delta_theta[];

float theta_v0;
float theta_v1;
float r_v0;
float r_v1;

float theta_v;
float r;
float delta_theta_v;
vec3 cartesian_position;

// Function that converts polar vector into a cartesian vector
vec3 to_cartesian(float r, float theta) {
		return vec3(r * cos(theta), r * sin(theta), 0.0f);
}

void tesselation_point(float d)
{
	float cosh_d = cosh(d);
	float sinh_d = sinh(d);

	float cosh_r_v = cosh_r_out[0] * cosh_d - sinh_r_out[0] * sinh_d * cos_alpha[0];
	r = acosh(cosh_r_v);
	float sinh_r_v = sinh(r);

	float cos_delta_theta_v = ((cosh_r_out[0] * cosh_r_v) - cosh_d) / (sinh_r_out[0] * sinh_r_v);
	if (cos_delta_theta_v > 1.0f) cos_delta_theta_v = 1.0f;
	if (cos_delta_theta_v < -1.0f) cos_delta_theta_v = -1.0f;
	delta_theta_v = acos(cos_delta_theta_v);

	theta_v = theta_v0 + final_direction[0] * delta_theta_v;

	// Convert from Polar to Cartesian coordinates
	cartesian_position = to_cartesian(r * in_k, theta_v);
}


void main()
{
 float u = gl_TessCoord.x;
 float d = d[0] * u;

 theta_v0 = gl_in[0].gl_Position.y;
 theta_v1 = gl_in[1].gl_Position.y;
 r_v0 = gl_in[0].gl_Position.x;
 r_v1 = gl_in[1].gl_Position.x;

 if (u!=0.0f && u!=1.0f && r_v0 != 0.0f && r_v1 != 0.0f)
 {
	tesselation_point(d);
 }
 else
 {
    // for a line aligned to a geodesic going through the origin, don't desselate  
	if(u<0.5f) cartesian_position = to_cartesian(r_v0, theta_v0);
	else cartesian_position = to_cartesian(r_v1, theta_v1);
 }
 // Transform the vertex spatial position using 
 gl_Position = u_view_projection * u_transform * vec4(cartesian_position, 1.0f);
} 
