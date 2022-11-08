#version 400 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_strips;
uniform int num_segments;

layout( vertices=2 ) out;

uniform vec3 in_object_position;
uniform float in_k;

out float lambda_0[];
out float lambda_1[];
out float cot_alpha_0[];

out float delta_theta[];

float theta_v0;
float theta_v1;
float r_v0;
float r_v1;

void tesselation()
{
	// convert to latitude / longitude
	// long = mod(theta + 180, 360) - 180
	// lat = 90 - r

	float phi_1 = m_pi / 2.f - r_v0;
	float phi_2 = m_pi / 2.f - r_v1;
	lambda_1[gl_InvocationID] = mod(theta_v0 + m_pi, 2.f * m_pi) - m_pi;
	float lambda_2 = mod(theta_v1 + m_pi, 2.f * m_pi) - m_pi;

	float sin_phi_1 = sin(phi_1);
	float cos_phi_1 = cos(phi_1);
	float sin_phi_2 = sin(phi_2);
	float cos_phi_2 = cos(phi_2);

	// calculate the preliminaries
	float lambda_12 = lambda_2 - lambda_1[0];
	if (lambda_12 < -m_pi)
		lambda_12 = 2.f * m_pi + lambda_12;
	if (lambda_12 > m_pi)
		lambda_12 = lambda_12 - 2.f * m_pi;

	float alpha_1 = atan(cos_phi_2 * sin(lambda_12), cos_phi_1 * sin_phi_2 - sin_phi_1 * cos_phi_2 * cos(lambda_12));
	float sin_alpha_1 = sin(alpha_1);
	float cos_alpha_1 = cos(alpha_1);

	float alpha_0 = atan(sin_alpha_1 * cos_phi_1, sqrt(cos_alpha_1 * cos_alpha_1 + sin_alpha_1 * sin_alpha_1 * sin_phi_1 * sin_phi_1));
	cot_alpha_0[gl_InvocationID] = 1 / tan(alpha_0);

	float tan_phi_1 = tan(phi_1);

	float sigma_01 = atan(tan_phi_1, cos(alpha_1));

	float lambda_01 = atan(sin(alpha_0) * sin(sigma_01), cos(sigma_01));

	lambda_0[gl_InvocationID] = lambda_1[0] - lambda_01;

	delta_theta[gl_InvocationID] = lambda_12;
}

void main()
{

 // Pass along the vertex position unmodified
 gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

 theta_v0 = gl_in[0].gl_Position.y;
 theta_v1 = gl_in[1].gl_Position.y;
 r_v0 = gl_in[0].gl_Position.x / in_k;
 r_v1 = gl_in[1].gl_Position.x / in_k;

 if(!(r_v0 == 0.0f || r_v1 == 0.0f)) 
 {
	if(gl_InvocationID == 0) {

		tesselation();
	}

	// Define the tessellation levels (this works on
	// ATI Catalyst drivers as of this writing, you may
	// need to swap these)

 
	gl_TessLevelOuter[1] = float(num_segments);
 }
 else
 {
	gl_TessLevelOuter[1] = 1.0f;
 }

 

 gl_TessLevelOuter[0] = float(num_strips);
}
