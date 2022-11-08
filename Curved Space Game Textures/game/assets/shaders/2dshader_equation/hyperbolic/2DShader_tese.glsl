#version 400 core

#define m_pi 3.1415926535897932384626433832795

layout( isolines ) in;

uniform int num_segments;
uniform float in_k;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

in float centre_x[];
in float centre_y[];
in float radius[];
in float v_theta_local[];

in float delta_theta[];

float theta_v0;
float theta_v1;
float r_v0;
float r_v1;

vec3 cartesian_position;

// Function that converts polar vector into a cartesian vector
vec3 to_cartesian(float r, float theta) {
		return vec3(r * cos(theta), r * sin(theta), 0.0f);
}

void tesselation_point(float d)
{
	vec3 new_poincare_cartesian = to_cartesian(radius[0], v_theta_local[0] + d) + vec3(centre_x[0], centre_y[0], 0.f);

	float new_r_poincare = sqrt(new_poincare_cartesian.x * new_poincare_cartesian.x + new_poincare_cartesian.y * new_poincare_cartesian.y);
	float new_theta_poincare = atan(new_poincare_cartesian.y, new_poincare_cartesian.x);

	cartesian_position = to_cartesian(2.f * atanh(new_r_poincare) * in_k, new_theta_poincare);

}

void main()
{
 float u = gl_TessCoord.x;
 float d = delta_theta[0] * u;

 theta_v0 = gl_in[0].gl_Position.y;
 theta_v1 = gl_in[1].gl_Position.y;
 r_v0 = gl_in[0].gl_Position.x;
 r_v1 = gl_in[1].gl_Position.x;

 if (u!=0.0f && u!=1.0f && r_v0 != 0.0f && r_v1 != 0.0f)
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
