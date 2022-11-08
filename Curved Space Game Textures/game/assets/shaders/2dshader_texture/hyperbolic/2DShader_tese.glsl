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
float cosh_r0;
float cosh_r1;
float sinh_r0;
float sinh_r1;

float theta_i;
float r_i;
float delta_theta_i;
vec3 cartesian_position;


// Function that converts polar vector into a cartesian vector
vec3 to_cartesian(float r, float theta) {
		return vec3(r * cos(theta), r * sin(theta), 0.0f);
}

void vertex_prime(float r_local, int n, inout float cosh_r, inout float sinh_r, inout float r, inout float delta_theta, inout float theta)
{
	float r_c = in_object_position.x / in_k;

	if(((es_beta[n] < 0.0001f || abs(es_beta[n] - 2.f * m_pi) < 0.0001f) && r_c - r_local == 0.f) || (abs(es_beta[n] - m_pi) < 0.0001f && r_c + r_local == 0.f))
	{
		r = 0.0f;
		cosh_r = cosh(0.0f);
		sinh_r = sinh(0.0f);
		delta_theta = 0.0f;
	}
	else
	{
		float sinh_r_c = sinh(r_c);
		float sinh_r_local = sinh(r_local);

		float cosh_r_c = cosh(r_c);
		float cosh_r_local = cosh(r_local);
		float cos_beta = cos(es_beta[n]);
		
	
		// using Hyperbolic Law of Cosines
		cosh_r = cosh_r_c * cosh_r_local - sinh_r_c * sinh_r_local * cos_beta;
		r = acosh(cosh_r);
		sinh_r = sinh(r);

		float cos_delta_theta = ((cosh_r_c * cosh_r) - cosh_r_local) / (sinh_r_c * sinh_r);
		if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
		if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;

		delta_theta = acos(cos_delta_theta);
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

	float cosh_edge = cosh_r0 * cosh_r1 - sinh_r0 * sinh_r1 * cos(delta_theta);
	float edge = acosh(cosh_edge);
	float sinh_edge = sinh(edge);

	float d = u * edge;
	float cosh_d = cosh(d);
	float sinh_d = sinh(d);

	float cos_alpha = (cosh_r0 * cosh_edge - cosh_r1) / (sinh_r0 * sinh_edge);

	float cosh_r_i = cosh_r0 * cosh_d - sinh_r0 * sinh_d * cos_alpha;
	r_i = acosh(cosh_r_i);
	float sinh_r_i = sinh(r_i);

	float cos_delta_theta_i = ((cosh_r0 * cosh_r_i) - cosh_d) / (sinh_r0 * sinh_r_i);
	if (cos_delta_theta_i > 1.0f) cos_delta_theta_i = 1.0f;
	if (cos_delta_theta_i < -1.0f) cos_delta_theta_i = -1.0f;
	delta_theta_i = acos(cos_delta_theta_i);

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
			vertex_prime(r0_prime_local, 0, cosh_r0, sinh_r0, r0, delta_theta0, theta0);

			cartesian_position = to_cartesian(r0 * in_k, theta0);
		}
		else if(gl_TessCoord.x == 0.f)
		{
			float r1_prime_local = (1.f - gl_TessCoord.z) * es_position_local[1].x;
			vertex_prime(r1_prime_local, 1, cosh_r1, sinh_r1, r1, delta_theta1, theta1);

			cartesian_position = to_cartesian(r1 * in_k, theta1);
		}
		else
		{
			float r0_prime_local = (1.f - gl_TessCoord.z) * es_position_local[0].x;
			float r1_prime_local = (1.f - gl_TessCoord.z) * es_position_local[1].x;

			vertex_prime(r0_prime_local, 0, cosh_r0, sinh_r0, r0, delta_theta0, theta0);
			vertex_prime(r1_prime_local, 1, cosh_r1, sinh_r1, r1, delta_theta1, theta1);

			float u = gl_TessCoord.y / (gl_TessCoord.x + gl_TessCoord.y);

			tesselation_point(u);
		}
	}

	fs_tex_coord = es_tex_coord[0] * gl_TessCoord.x + es_tex_coord[1] * gl_TessCoord.y + es_tex_coord[2] * gl_TessCoord.z;
	gl_Position = u_view_projection * vec4(cartesian_position, 1.0f);
}










