#version 430 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_segments;
uniform float in_k;
uniform vec3 in_object_position;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

layout( triangles, equal_spacing, ccw ) in;

in vec2 es_tex_coord[];
in vec3 es_position[];
in vec3 es_position_local[];

in int es_final_direction[];
in float es_delta_theta[];
in float es_beta[];

out vec2 fs_tex_coord;

float theta_v;
float r;
float delta_theta_v;
vec3 cartesian_position;


// Function that converts polar vector into a cartesian vector
vec3 to_cartesian(float r, float theta) {
		return vec3(r * cos(theta), r * sin(theta), 0.0f);
}

void tesselation_point(float r0, float r1, float theta0, float theta1, float u)
{
	float cosh_r0 = cosh(r0);
	float cosh_r1 = cosh(r1);
	float sinh_r0 = sinh(r0);
	float sinh_r1 = sinh(r1);

	float cosh_edge = cosh_r0 * cosh_r1 - sinh_r0 * sinh_r1 * cos(es_delta_theta[2]);
	float edge = acosh(cosh_edge);
	float sinh_edge = sinh(edge);

	float d = u * edge;
	float cosh_d = cosh(d);
	float sinh_d = sinh(d);

	float cos_alpha = (cosh_r0 * cosh_edge - cosh_r1) / (sinh_r0 * sinh_edge);

	float cosh_r_v = cosh_r0 * cosh_d - sinh_r0 * sinh_d * cos_alpha;
	r = acosh(cosh_r_v);
	float sinh_r_v = sinh(r);

	float cos_delta_theta_v = ((cosh_r0 * cosh_r_v) - cosh_d) / (sinh_r0 * sinh_r_v);
	if (cos_delta_theta_v > 1.0f) cos_delta_theta_v = 1.0f;
	if (cos_delta_theta_v < -1.0f) cos_delta_theta_v = -1.0f;
	delta_theta_v = acos(cos_delta_theta_v);

	theta_v = theta0 + es_final_direction[2] * delta_theta_v;

	// Convert from Polar to Cartesian coordinates
	cartesian_position = to_cartesian(r * in_k, theta_v);
}

void main()  
{
	//	float u = gl_TessCoord.x;
	//	float d = es_edge[2] * u;

	//	if (u!=0.0f && u!=1.0f && es_position[0].x != 0.0f && es_position[1].x != 0.0f) { tesselation_point(d);	}
	//	else {
				// for a line aligned to a geodesic going through the origin, don't desselate  
		//	if(u<0.5f) cartesian_position = to_cartesian(es_position[0].x, es_position[0].y);
		//	else cartesian_position = to_cartesian(es_position[1].x, es_position[1].y);	}
				// Transform the vertex spatial position using 
	//	gl_Position = u_view_projection * u_transform * vec4(cartesian_position, 1.0f);

	
	if(gl_TessCoord.z == 1.f)
	{
		cartesian_position = to_cartesian(in_object_position.x, in_object_position.y);
	} else if(gl_TessCoord.x == 0.f)
	{
		cartesian_position = to_cartesian(es_position[1].x * (1.f - gl_TessCoord.z) * in_k, es_position[1].y);
	}
	else if(gl_TessCoord.y == 0.f)
	{
		cartesian_position = to_cartesian(es_position[0].x * (1.f - gl_TessCoord.z) * in_k, es_position[0].y);
	}
	else
	{
		float r0_prime = (1.f - gl_TessCoord.z) * es_position[0].x;
		float r1_prime = (1.f - gl_TessCoord.z) * es_position[1].x;

		float u = gl_TessCoord.y / (gl_TessCoord.x + gl_TessCoord.y) ;

		tesselation_point(r0_prime, r1_prime, es_position[0].y, es_position[1].y, u);
	}

	fs_tex_coord = es_tex_coord[0] * gl_TessCoord.x + es_tex_coord[1] * gl_TessCoord.y + es_tex_coord[2] * gl_TessCoord.z;
	vec3 position = (gl_TessCoord.x * es_position[0]) + (gl_TessCoord.y * es_position[1]) + (gl_TessCoord.z * es_position[2]);
	//gl_Position = u_view_projection * vec4(position, 1.0);
	gl_Position = u_view_projection * vec4(cartesian_position, 1.0f);
}










