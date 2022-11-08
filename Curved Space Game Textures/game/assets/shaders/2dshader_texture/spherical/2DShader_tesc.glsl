#version 430 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_strips;
uniform int num_segments;

layout(vertices = 3) out;

uniform vec3 in_object_position;
uniform float in_k;

in vec2 cs_tex_coord[];
in vec3 cs_position[];
in vec3 cs_position_local[];
in float cs_cos_r_v[];
in float cs_sin_r_v[];
in float cs_beta[];
in int cs_direction[];

out vec2 es_tex_coord[];
out vec3 es_position[];
out vec3 es_position_local[];

out int es_direction[];
out float es_delta_theta[];
out float es_beta[];

// Function that converts polar vector into a cartesian vector
vec3 toCartesian(float R, float Theta) {
		return vec3(R * cos(Theta), R * sin(Theta), 0.0f);
}

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
	float edge;

	if(delta_theta<m_pi/10.0f || delta_theta > m_pi - (m_pi/10.0f)) {
		float hav_delta_theta = hav(delta_theta);

		float hav_edge = hav(cs_position[0].x/in_k - cs_position[1].x/in_k) + cs_sin_r_v[0] * cs_sin_r_v[1] * hav_delta_theta;
		edge = ahav(hav_edge);

	} else {
		float cos_delta_theta = cos(delta_theta);

		float cos_edge = cs_cos_r_v[0] * cs_cos_r_v[1] + cs_sin_r_v[0] * cs_sin_r_v[1] * cos_delta_theta;
		edge = acos(cos_edge);
	}
}

void main()  
{
	if (!(gl_InvocationID==2))
	{
		es_tex_coord[gl_InvocationID] = cs_tex_coord[gl_InvocationID];
		es_position[gl_InvocationID] = cs_position[gl_InvocationID];
		es_position_local[gl_InvocationID] = cs_position_local[gl_InvocationID];
		es_beta[gl_InvocationID] = cs_beta[gl_InvocationID];

		es_direction[gl_InvocationID] = cs_direction[gl_InvocationID];
	}
	else
	{
		es_tex_coord[gl_InvocationID] = vec2(0.5f, 0.5f);
		es_position[gl_InvocationID] = in_object_position;
		es_position_local[gl_InvocationID] = in_object_position;
		es_beta[gl_InvocationID] = 0.0f;

		if(!(cs_position[0].x == 0.0f || cs_position[1].x == 0.0f)) 
		{
			float delta_theta = abs(cs_position[0].y - cs_position[1].y);
			es_delta_theta[gl_InvocationID] = delta_theta;

			tesselation(delta_theta);	

			int direction = 1;
			float dif_theta_0 = abs(in_object_position.y - cs_position[0].y);
			float dif_theta_1 = abs(in_object_position.y - cs_position[1].y);

			if (cs_position[1].y < cs_position[0].y && (dif_theta_0 + dif_theta_1) < m_pi) direction = -1;
			if ((dif_theta_0 + dif_theta_1) > m_pi && in_object_position.x > 200) direction = -1;

			es_direction[gl_InvocationID] = direction;

			gl_TessLevelInner[0] = num_strips;

			gl_TessLevelOuter[0] = num_segments;
			gl_TessLevelOuter[1] = num_segments;
			gl_TessLevelOuter[2] = num_segments;
		}
		else
		{
			gl_TessLevelInner[0] = 1.0;

			gl_TessLevelOuter[0] = 1.0;
			gl_TessLevelOuter[1] = 1.0;
			gl_TessLevelOuter[2] = 1.0;
		}
	}
}
