#version 430 core

#define m_pi 3.1415926535897932384626433832795

uniform float in_k;
uniform mat4 u_transform;
uniform vec3 in_object_position;
uniform float in_object_rotation;

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;  

out vec2 cs_tex_coord;
out vec3 cs_position;
out vec3 cs_position_local;
out float cs_cos_r_v;
out float cs_sin_r_v;
out float cs_beta;
out int cs_direction;

float delta_theta_v;
float theta_v;
float r_v;

// haversine function
float hav(float value)
{
	return sin(value/2.0f)*sin(value/2.0f);
}

// archaversine function
float ahav(float value)
{
	return 2.0f*asin(sqrt(value));
}

// function to constrain an angle to a range 0 to 2pi
float constrain_angle_rad(float angle)
{
	angle = mod(angle, 2 * m_pi);
	if (angle < 0)
		angle += 2 * m_pi;
	return angle;
}

void vertices(float r_c, float r_local)
{
	if(((cs_beta < 0.0001f || abs(cs_beta - 2.f * m_pi) < 0.0001f) && r_c - r_local == 0.f) || (abs(cs_beta - m_pi) < 0.0001f && r_c + r_local == 0.f))
	{
		r_v = 0.0f;
		cs_direction = 1;
		cs_cos_r_v = cos(0.0f);
		cs_sin_r_v = sin(0.0f);
		delta_theta_v = 0.0f;
	}
	else
	{
		float sin_r_c = sin(r_c);
		
		float sin_r_local = sin(r_local);

		if(cs_beta < m_pi / 60.0f || cs_beta > m_pi - (m_pi / 60.0f)) {
			float hav_beta = hav(cs_beta);
		
			float hav_r_v = hav(r_c - r_local) + sin_r_c * sin_r_local * hav_beta;
			r_v = ahav(hav_r_v);
			cs_cos_r_v = cos(r_v);
			cs_sin_r_v = sin(r_v);

			float hav_r_local = hav(r_local);
			
			float hav_delta_theta_v = (hav_r_local - hav(r_c - r_v)) / (sin_r_c * cs_sin_r_v);
			if (hav_delta_theta_v > 1.0f) hav_delta_theta_v = 1.0f;
			if (hav_delta_theta_v < 0.0f) hav_delta_theta_v = 0.0f;

			delta_theta_v = ahav(hav_delta_theta_v);
		} else {
			float cos_beta = cos(cs_beta);
			float cos_r_c = cos(r_c);
			float cos_r_local = cos(r_local);
			
			cs_cos_r_v =  cos_r_c * cos_r_local + sin_r_c * sin_r_local * cos_beta;
			r_v = acos(cs_cos_r_v);
			cs_sin_r_v = sin(r_v);
			
			float cos_delta_theta_v = (cos_r_local - (cos_r_c * cs_cos_r_v)) / (sin_r_c * cs_sin_r_v);
			if (cos_delta_theta_v > 1.0f) cos_delta_theta_v = 1.0f;
			if (cos_delta_theta_v < -1.0f) cos_delta_theta_v = -1.0f;

			delta_theta_v = acos(cos_delta_theta_v);

		}
	}
}

// Function which calculates the vertex position in polar coordinates
vec3 curvature_vertices(float k, float rotation_angle, vec3 object_position, vec3 vertex_position)
{
	float r_c = object_position.x / k;

	cs_direction = 1;
	cs_beta = constrain_angle_rad(- (vertex_position.y + rotation_angle));
	if (cs_beta > float(m_pi)) { cs_beta = float(2 * m_pi) - cs_beta; cs_direction *= -1; }

	float r_local = vertex_position.x / k;
	
	vertices(r_c, r_local);

	theta_v = object_position.y + cs_direction * delta_theta_v;
	
	return vec3(r_v, theta_v, 0.0f);
}

void main()  
{
	cs_cos_r_v = 0.0f;
	cs_sin_r_v = 0.0f;
	cs_position_local = vec3(a_position.x / in_k, a_position.y, 0.0f);

	// Find the vertex position in Polar coordinates
	if(in_object_position.x != 0.0f)
	{
		cs_position = curvature_vertices(in_k, in_object_rotation, in_object_position, a_position);
	}
	else
	{
		r_v = a_position.x / in_k;
		cs_position = vec3(a_position.x, a_position.y + in_object_rotation, 0.0f);

		cs_cos_r_v = cos(r_v);
		cs_sin_r_v = sin(r_v);

		cs_direction = -1;
		delta_theta_v = 0.0f;
	}
    cs_tex_coord = a_tex_coord;
}
