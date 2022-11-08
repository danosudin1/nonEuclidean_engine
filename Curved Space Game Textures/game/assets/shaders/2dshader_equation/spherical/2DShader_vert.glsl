#version 400 core

#define m_pi 3.1415926535897932384626433832795

uniform float in_k;
uniform vec3 in_object_position;
uniform float in_object_rotation;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 in_position;

int direction;

float delta_theta;
float theta;
float r;


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


void vertices(float beta, float r_c, float r_local)
{
	if(((beta < 0.0001f || abs(beta - 2.f * m_pi) < 0.0001f) && r_c - r_local == 0.f) || (abs(beta - m_pi) < 0.0001f && r_c + r_local == 0.f))
	{
		r = 0.0f;
		direction = 1;
		delta_theta = 0.0f;
	}
	else
	{
		float sin_r_c = sin(r_c);
		
		float sin_r_local = sin(r_local);

		if(beta < m_pi / 60.0f || beta > m_pi - (m_pi / 60.0f)) {
			float hav_beta = hav(beta);
		
			float hav_r = hav(r_c - r_local) + sin_r_c * sin_r_local * hav_beta;
			r = ahav(hav_r);
			float cos_r = cos(r);
			float sin_r = sin(r);

			float hav_r_local = hav(r_local);
			
			float hav_delta_theta = (hav_r_local - hav(r_c - r)) / (sin_r_c * sin_r);
			if (hav_delta_theta > 1.0f) hav_delta_theta = 1.0f;
			if (hav_delta_theta < 0.0f) hav_delta_theta = 0.0f;

			delta_theta = ahav(hav_delta_theta);
		} else {
			float cos_beta = cos(beta);
			float cos_r_c = cos(r_c);
			float cos_r_local = cos(r_local);
			
			float cos_r =  cos_r_c * cos_r_local + sin_r_c * sin_r_local * cos_beta;
			r = acos(cos_r);
			float sin_r = sin(r);
			
			float cos_delta_theta = (cos_r_local - (cos_r_c * cos_r)) / (sin_r_c * sin_r);
			if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
			if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;

			delta_theta = acos(cos_delta_theta);
		}
	}
}

// Function which calculates the vertex position in polar coordinates
vec3 curvature_vertices(float k, float rotation_angle, vec3 object_position, vec3 vertex_position)
{

	float r_c = object_position.x / k;

	direction = 1;
	float beta = constrain_angle_rad(- (vertex_position.y + rotation_angle));
	if (beta > float(m_pi)) { beta = float(2 * m_pi) - beta; direction *= -1; }

	


	float r_local = vertex_position.x / k;
	
	vertices(beta, r_c, r_local);

	theta = object_position.y + direction * delta_theta;
	
	return vec3(r*k, theta, 0.0f);
	
}


// This is the entry point into the vertex shader
void main()
{

	vec3 polar_position;

	// Find the vertex position in Polar coordinates
	if(in_object_position.x != 0.0f)
		polar_position = curvature_vertices(in_k, in_object_rotation, in_object_position, in_position);
	else
	{
		float r = in_position.x / in_k;
		polar_position = vec3(in_position.x, in_position.y + in_object_rotation, 0.0f);

		direction = -1;
		delta_theta = 0.0f;
	}

	// Transform the vertex spatial position using 
	gl_Position = vec4(polar_position, 1.0f);
	
} 

