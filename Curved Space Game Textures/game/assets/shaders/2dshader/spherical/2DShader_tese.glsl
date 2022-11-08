#version 400 core

#define m_pi 3.1415926535897932384626433832795

layout( isolines ) in;

uniform int num_segments;
uniform float in_k;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

in float edge[];
in float cos_alpha[];

in float cos_r_out[];
in float sin_r_out[];

in int final_direction[];

in float delta_theta_out[];

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

// haversine function
float hav(float value) {
	return sin(value/2.0f)*sin(value/2.0f);
}

// archaversine function
float ahav(float value) {
	return 2.0f*asin(sqrt(value));
}

void tesselation_point(float d)
{
	float sin_d = sin(d);

	// if the angle is small, use a more precise haversine rule
	if(delta_theta_out[0] < m_pi / 10.0f || delta_theta_out[0] > m_pi - (m_pi / 10.0f))
	{
		float hav_d = hav(d);
		float hav_r_v = hav(r_v0/in_k - d) + sin_r_out[0] * sin_d * cos_alpha[0];
		r = ahav(hav_r_v);

		float sin_r_v = sin(r);
	
		float hav_delta_theta_v = (hav_d - hav(r_v0/in_k - r)) / (sin_r_out[0] * sin_r_v);
		if (hav_delta_theta_v > 1.0f) hav_delta_theta_v = 1.0f;
		if (hav_delta_theta_v < 0.0f) hav_delta_theta_v = 0.0f;
		delta_theta_v = ahav(hav_delta_theta_v);
			
	}
	// otherwise use hyperbolic cosine rule
	else
	{
		float cos_d = cos(d);
		float cos_r_v = cos_r_out[0] * cos_d + sin_r_out[0] * sin_d * cos_alpha[0];
		r = acos(cos_r_v);
		float sin_r_v = sin(r);

		float cos_delta_theta_v = (cos_d - (cos_r_out[0] * cos_r_v)) / (sin_r_out[0] * sin_r_v);
		if (cos_delta_theta_v > 1.0f) cos_delta_theta_v = 1.0f;
		if (cos_delta_theta_v < -1.0f) cos_delta_theta_v = -1.0f;
		delta_theta_v = acos(cos_delta_theta_v);
	}

	theta_v = theta_v0 + final_direction[0] * delta_theta_v;

	//Convert from Polar to Cartesian coordinates
	cartesian_position = to_cartesian(r*in_k, theta_v);
}

void main()
{
 float u = gl_TessCoord.x;
 float d = edge[0] * u;

 theta_v0 = gl_in[0].gl_Position.y;
 theta_v1 = gl_in[1].gl_Position.y;
 r_v0 = gl_in[0].gl_Position.x;
 r_v1 = gl_in[1].gl_Position.x;

 if(u!=0.0f && u!=1.0f && r_v0 != 0.0f && r_v1 != 0.0f)
 {
	tesselation_point(d);
 }
 else
 {
    // for a k = 0 curvature, just assign the  
	if(u<0.5f) cartesian_position = to_cartesian(r_v0, theta_v0);
	else cartesian_position = to_cartesian(r_v1, theta_v1);
 }
 // Transform the vertex spatial position using 
 gl_Position = u_view_projection * u_transform * vec4(cartesian_position, 1.0f);
} 
