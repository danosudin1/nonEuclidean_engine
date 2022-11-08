#version 430 core

#define m_pi 3.1415926535897932384626433832795

uniform mat4 u_transform;
uniform float in_k;
uniform vec3 in_object_position;
uniform float in_object_rotation;

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;  

out vec2 cs_tex_coord;
out vec3 cs_position;
out vec3 cs_position_local;
out float cs_cosh_r;
out float cs_sinh_r;
out float cs_beta;

int direction;

float delta_theta;
float theta;
float r;

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
		r = 0.0f;
		direction = 1;
		cs_cosh_r = cos(0.0f);
		cs_sinh_r = sin(0.0f);
		delta_theta = 0.0f;
	}
	else
	{
		float cosh_r_c = cosh(r_c);
		float sinh_r_c = sinh(r_c);

		float cos_beta = cos(cs_beta);

		float cosh_r_local = cosh(r_local);
		float sinh_r_local = sinh(r_local);
	
		// using Hyperbolic Law of Cosines
		cs_cosh_r = cosh_r_c * cosh_r_local - sinh_r_c * sinh_r_local * cos_beta;
		r = acosh(cs_cosh_r);
		cs_sinh_r = sinh(r);

		float cos_delta_theta = ((cosh_r_c * cs_cosh_r) - cosh_r_local) / (sinh_r_c * cs_sinh_r);
		if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
		if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;
		delta_theta = acos(cos_delta_theta);
	}
}

// Function which calculates the vertex position in polar coordinates
vec3 curvature_vertices(float k, float rotation_angle, vec3 object_position, vec3 vertex_position)
{
	float r_c = object_position.x / k;

	direction = 1;
	cs_beta = constrain_angle_rad(- (vertex_position.y + rotation_angle));
	if (cs_beta > float(m_pi)) { cs_beta = float(2 * m_pi) - cs_beta; direction *= -1; }

	float r_local = vertex_position.x / k;
	
	vertices(r_c, r_local);

	theta = object_position.y + direction * delta_theta;
	
	return vec3(r, theta, 0.0f);
}

// Function that converts polar vector into a cartesian vector
vec3 toCartesian(float R, float Theta) {
		return vec3(R * cos(Theta), R * sin(Theta), 0.0f);
}

void main()  
{
	cs_cosh_r = 0.0f;
	cs_sinh_r = 0.0f;

	if(in_object_position.x != 0.0f)
	{
		cs_position = curvature_vertices(in_k, in_object_rotation, in_object_position, a_position);
	}
	else
	{
		float r = a_position.x / in_k;
		cs_position = vec3(a_position.x, a_position.y + in_object_rotation, 0.0f);
		
		cs_cosh_r = cosh(r);
		cs_sinh_r = sinh(r);

		direction = -1;
		delta_theta = 0.0f;
	}

    cs_tex_coord = a_tex_coord;
}














