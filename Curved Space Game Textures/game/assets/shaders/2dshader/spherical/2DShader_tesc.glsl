#version 400 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_strips;
uniform int num_segments;

layout( vertices=2 ) out;

uniform vec3 in_object_position;
uniform float in_k;

in float cos_r[];
in float sin_r[];

out float edge[];
out float cos_alpha[];

out float cos_r_out[];
out float sin_r_out[];
out int final_direction[];
out float delta_theta_out[];

float theta_v0;
float theta_v1;
float theta_obj;
float r_v0;
float r_v1;
float r_obj;


// haversine function
float hav(float value) {
	return sin(value/2.0f)*sin(value/2.0f);
}


// archaversine function
float ahav(float value) {
	return 2.0f*asin(sqrt(value));
}


void tesselation(float delta_theta)
{
	if(delta_theta<m_pi/10.0f || delta_theta > m_pi - (m_pi/10.0f)) {
		float hav_delta_theta = hav(delta_theta);

		float hav_edge = hav(r_v0/in_k - r_v1/in_k) + sin_r[0] * sin_r[1] * hav_delta_theta;
		edge[gl_InvocationID] = ahav(hav_edge);
		float sin_edge = sin(edge[gl_InvocationID]);

		cos_alpha[gl_InvocationID] = (hav(r_v1/in_k) - hav(r_v0/in_k - edge[gl_InvocationID])) / (sin_r[0] * sin_edge);
	} else {
		float cos_delta_theta = cos(delta_theta);

		float cos_edge = cos_r[0] * cos_r[1] + sin_r[0] * sin_r[1] * cos_delta_theta;
		edge[gl_InvocationID] = acos(cos_edge);
		float sin_edge = sin(edge[gl_InvocationID]);

		cos_alpha[gl_InvocationID] = (cos_r[1] - (cos_r[0] * cos_edge)) / (sin_r[0] * sin_edge);
	}
}

void main()
{

 // Pass along the vertex position unmodified
 gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

 cos_r_out[gl_InvocationID] = cos_r[gl_InvocationID];
 sin_r_out[gl_InvocationID] = sin_r[gl_InvocationID];

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
		edge[gl_InvocationID] = 0.0f;
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
