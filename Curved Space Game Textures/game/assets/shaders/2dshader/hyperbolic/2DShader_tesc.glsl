#version 400 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_strips;
uniform int num_segments;

layout( vertices=2 ) out;

uniform vec3 in_object_position;
uniform float in_k;

in float cosh_r[];
in float sinh_r[];

out float d[];
out float cos_alpha[];

out float cosh_r_out[];
out float sinh_r_out[];
out int final_direction[];
out float delta_theta_out[];

float theta_v0;
float theta_v1;
float theta_obj;
float r_v0;
float r_v1;
float r_obj;



void tesselation(float delta_theta)
{
	float cos_delta_theta = cos(delta_theta);
		
	float cosh_d = cosh_r[0] * cosh_r[1] - sinh_r[0] * sinh_r[1] * cos_delta_theta;

	d[gl_InvocationID] = acosh(cosh_d);

	float sinh_d = sinh(d[gl_InvocationID]);

	cos_alpha[gl_InvocationID] = ((cosh_r[0] * cosh_d) - cosh_r[1]) / (sinh_r[0] * sinh_d);
}


void main()
{

 // Pass along the vertex position unmodified
 gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

 cosh_r_out[gl_InvocationID] = cosh_r[gl_InvocationID];
 sinh_r_out[gl_InvocationID] = sinh_r[gl_InvocationID];

 theta_v0 = gl_in[0].gl_Position.y;
 theta_v1 = gl_in[1].gl_Position.y;
 theta_obj = in_object_position.y;
 r_obj = in_object_position.x;
 r_v0 = gl_in[0].gl_Position.x;
 r_v1 = gl_in[1].gl_Position.x;

 if(!(r_v0 == 0.0f || r_v1 == 0.0f)) 
 {
	if(gl_InvocationID == 0) {
		float delta_theta = abs(theta_v0 - theta_v1);
		delta_theta_out[gl_InvocationID] = delta_theta;

		tesselation(delta_theta);	

		int direction = 1;
		float dif_theta_0 = abs(theta_obj - theta_v0);
		float dif_theta_1 = abs(theta_obj - theta_v1);

		if (theta_v1 < theta_v0 && (dif_theta_0 + dif_theta_1) < m_pi) direction = -1;
		if ((dif_theta_0 + dif_theta_1) > m_pi && r_obj > 200) direction = -1;

		final_direction[gl_InvocationID] = direction;

	 }
	 else
	 {
		d[gl_InvocationID] = 0.0f;
		cos_alpha[gl_InvocationID] = 0.0f;
		final_direction[gl_InvocationID] = 0;
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
