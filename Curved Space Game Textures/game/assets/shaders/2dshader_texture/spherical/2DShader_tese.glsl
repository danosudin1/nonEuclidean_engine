#version 430 core

#define m_pi 3.1415926535897932384626433832795

uniform float in_k;
uniform vec3 in_object_position;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

layout( triangles, equal_spacing, ccw ) in;

in vec2 es_tex_coord[];
in vec3 es_position[];
in vec3 es_position_local[];

in int es_direction[];
in float es_delta_theta[];
in float es_beta[];

out vec2 fs_tex_coord;

float r0;
float r1;
float delta_theta0;
float delta_theta1;
float theta0;
float theta1;
float cos_r0;
float cos_r1;
float sin_r0;
float sin_r1;

float theta_i;
float r_i;
float delta_theta_i;
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

void vertex_prime(float r_local, int n, inout float cos_r, inout float sin_r, inout float r, inout float delta_theta, inout float theta)
{
	float r_c = in_object_position.x / in_k;

	if(((es_beta[n] < 0.0001f || abs(es_beta[n] - 2.f * m_pi) < 0.0001f) && r_c - r_local == 0.f) || (abs(es_beta[n] - m_pi) < 0.0001f && r_c + r_local == 0.f))
	{
		r = 0.0f;
		cos_r = cos(0.0f);
		sin_r = sin(0.0f);
		delta_theta = 0.0f;
	}
	else
	{
		
		float sin_r_c = sin(r_c);
		float sin_r_local = sin(r_local);

		if(es_beta[n] < m_pi / 60.0f || es_beta[n] > m_pi - (m_pi / 60.0f)) {

			float hav_beta = hav(es_beta[n]);
			float hav_r = hav(r_c - r_local) + sin_r_c * sin_r_local * hav_beta;

			r = ahav(hav_r);
			cos_r = cos(r);
			sin_r = sin(r);

			float hav_r_local = hav(r_local);
			
			float hav_delta_theta = (hav_r_local - hav(r_c - r)) / (sin_r_c * sin_r);
			if (hav_delta_theta > 1.0f) hav_delta_theta = 1.0f;
			if (hav_delta_theta < 0.0f) hav_delta_theta = 0.0f;

			delta_theta = ahav(hav_delta_theta);

		} else {

			float cos_r_c = cos(r_c);
			float cos_r_local = cos(r_local);
			float cos_beta = cos(es_beta[n]);
		
			// using Spherical Law of Cosines
			cos_r = cos_r_c * cos_r_local + sin_r_c * sin_r_local * cos_beta;
			r = acos(cos_r);
			sin_r = sin(r);

			float cos_delta_theta = (cos_r_local - (cos_r_c * cos_r)) / (sin_r_c * sin_r);
			if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
			if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;

			delta_theta = acos(cos_delta_theta);
		}

		theta = in_object_position.y + es_direction[n] * delta_theta;
	}
}

void tesselation_point(float u)
{
	int direction = 1;
	float dif_theta_0 = abs(in_object_position.y - theta0);
	float dif_theta_1 = abs(in_object_position.y - theta1);

	if (theta1 < theta0 && (dif_theta_0 + dif_theta_1) < m_pi) direction = -1;
	if ((dif_theta_0 + dif_theta_1) > m_pi && in_object_position.r > 200) direction = -1;

	float delta_theta = abs(theta1 - theta0);

	// if the angle is small, use a more precise haversine rule
	if(delta_theta < m_pi / 10.0f || delta_theta > m_pi - (m_pi / 10.0f))
	{
		float hav_edge = hav(r0 - r1) + sin_r0 * sin_r1 * hav(delta_theta);
		float edge = ahav(hav_edge);
		float sin_edge = sin(edge);
	
		float hav_alpha = (hav(r1) - hav(r0 - edge)) / (sin_r0 * sin_edge);
	
		float d = u * edge;
		float hav_d = hav(d);
		float sin_d = sin(d);
		float hav_r_i = hav(r0 - d) + sin_r0 * sin_d * hav_alpha;
		r_i = ahav(hav_r_i);

		float sin_r_i = sin(r_i);
	
		float hav_delta_theta_i = (hav_d - hav(r0 - r_i)) / (sin_r0 * sin_r_i);
		if (hav_delta_theta_i > 1.0f) hav_delta_theta_i = 1.0f;
		if (hav_delta_theta_i < 0.0f) hav_delta_theta_i = 0.0f;
		delta_theta_i = ahav(hav_delta_theta_i);
			
	}
	// otherwise use spherical cosine rule
	else
	{

		float cos_edge = cos_r0 * cos_r1 + sin_r0 * sin_r1 * cos(delta_theta);
		float edge = acos(cos_edge);
		float sin_edge = sin(edge);

		float d = u * edge;	
		float cos_d = cos(d);
		float sin_d = sin(d);

		float cos_alpha = (cos_r1 - (cos_r0 * cos_edge)) / (sin_r0 * sin_edge);
		
		float cos_r_i = cos_r0 * cos_d + sin_r0 * sin_d * cos_alpha;
		r_i = acos(cos_r_i);
		float sin_r_i = sin(r_i);

		float cos_delta_theta_i = (cos_d - (cos_r0 * cos_r_i)) / (sin_r0 * sin_r_i);
		if (cos_delta_theta_i > 1.0f) cos_delta_theta_i = 1.0f;
		if (cos_delta_theta_i < -1.0f) cos_delta_theta_i = -1.0f;
		delta_theta_i = acos(cos_delta_theta_i);
	}

	theta_i = theta0 + direction * delta_theta_i;

	// Convert from Polar to Cartesian coordinates
	cartesian_position = to_cartesian(r_i * in_k, theta_i);
}

void main()  
{
	if(gl_TessCoord.z == 1.f)
	{
		cartesian_position = to_cartesian(in_object_position.x, in_object_position.y);
	}
	else if(gl_TessCoord.y == 1.f)
	{
		cartesian_position = to_cartesian(es_position[1].x * in_k, es_position[1].y);
	}
	else if(gl_TessCoord.x == 1.f)
	{
		cartesian_position = to_cartesian(es_position[0].x * in_k, es_position[0].y);
	}
	else
	{
		if(gl_TessCoord.y == 0.f)
		{
			float r0_prime_local = (1.f - gl_TessCoord.z) * es_position_local[0].x;
			vertex_prime(r0_prime_local, 0, cos_r0, sin_r0, r0, delta_theta0, theta0);

			cartesian_position = to_cartesian(r0 * in_k, theta0);
		}
		else if(gl_TessCoord.x == 0.f)
		{
			float r1_prime_local = (1.f - gl_TessCoord.z) * es_position_local[1].x;
			vertex_prime(r1_prime_local, 1, cos_r1, sin_r1, r1, delta_theta1, theta1);

			cartesian_position = to_cartesian(r1 * in_k, theta1);
		}
		else
		{
			float r0_prime_local = (1.f - gl_TessCoord.z) * es_position_local[0].x;
			float r1_prime_local = (1.f - gl_TessCoord.z) * es_position_local[1].x;

			vertex_prime(r0_prime_local, 0, cos_r0, sin_r0, r0, delta_theta0, theta0);
			vertex_prime(r1_prime_local, 1, cos_r1, sin_r1, r1, delta_theta1, theta1);

			float u = gl_TessCoord.y / (gl_TessCoord.x + gl_TessCoord.y);

			tesselation_point(u);
		}
	}

	fs_tex_coord = es_tex_coord[0] * gl_TessCoord.x + es_tex_coord[1] * gl_TessCoord.y + es_tex_coord[2] * gl_TessCoord.z;
	gl_Position = u_view_projection * vec4(cartesian_position, 1.0f);
}










