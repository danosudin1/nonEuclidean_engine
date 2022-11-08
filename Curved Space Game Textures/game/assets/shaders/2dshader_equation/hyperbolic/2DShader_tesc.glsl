#version 400 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_strips;
uniform int num_segments;

layout( vertices=2 ) out;

uniform vec3 in_object_position;
uniform float in_k;

in float cosh_r[];
in float sinh_r[];

out float centre_x[];
out float centre_y[];
out float radius[];
out float v_theta_local[];

out float delta_theta[];

float theta_v0;
float theta_v1;
float r_v0;
float r_v1;

void tesselation()
{
	float r_poincare_vertex_0 = tanh(r_v0 / 2.f);
	float r_poincare_vertex_1 = tanh(r_v1 / 2.f);

	float v_x_global = r_poincare_vertex_0 * cos(theta_v0);
	float v_y_global = r_poincare_vertex_0 * sin(theta_v0);
	float u_x_global = r_poincare_vertex_1 * cos(theta_v1);
	float u_y_global = r_poincare_vertex_1 * sin(theta_v1);

	// find poincare geodesic circle equation. x^2 + y^2 + ax + by + 1 = 0 

	float a = (u_y_global * (v_x_global * v_x_global + v_y_global * v_y_global) - v_y_global * (u_x_global * u_x_global + u_y_global * u_y_global)
		+ u_y_global - v_y_global) / (u_x_global * v_y_global - u_y_global * v_x_global);

	float b = (v_x_global * (u_x_global * u_x_global + u_y_global * u_y_global) - u_x_global * (v_x_global * v_x_global + v_y_global * v_y_global)
		+ v_x_global - u_x_global) / (u_x_global * v_y_global - u_y_global * v_x_global);

	// Find centre of the circle and radius. As well as the distance (centre_r coordinate) to the centre from origin

	radius[gl_InvocationID] = sqrt(0.25f * a * a + 0.25f * b * b - 1);
	centre_x[gl_InvocationID] = -0.5f * a;
	centre_y[gl_InvocationID] = -0.5f * b;

	// point p, closest point to origin on the circumference of the circle. distance to p is (centre_dist - radius) / centre_dist

	// float p_x_global = centre_x * (centre_r[gl_InvocationID] - radius[gl_InvocationID]) / centre_r[gl_InvocationID];
	// float p_y_global = centre_y * (centre_r[gl_InvocationID] - radius[gl_InvocationID]) / centre_r[gl_InvocationID];

	// Find local theta coordinate of point p, v and u within the circle

	// float p_theta_local = atan(p_y_global - centre_y, p_x_global - centre_x);
	v_theta_local[gl_InvocationID] = atan(v_y_global - centre_y[gl_InvocationID], v_x_global - centre_x[gl_InvocationID]);
	float u_theta_local = atan(u_y_global - centre_y[gl_InvocationID], u_x_global - centre_x[gl_InvocationID]);
	// p_r_local, u_r_local, v_r_local is radius

	float delta_theta_temp = u_theta_local - v_theta_local[gl_InvocationID];
	if (delta_theta_temp > m_pi) delta_theta_temp -= 2.f * m_pi;
	if (delta_theta_temp < -m_pi) delta_theta_temp += 2.f * m_pi;
	delta_theta[gl_InvocationID] = delta_theta_temp;
}


void main()
{

 // Pass along the vertex position unmodified
 gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

 theta_v0 = gl_in[0].gl_Position.y;
 theta_v1 = gl_in[1].gl_Position.y;
 r_v0 = gl_in[0].gl_Position.x / in_k;
 r_v1 = gl_in[1].gl_Position.x / in_k;

 if(!(r_v0 == 0.0f || r_v1 == 0.0f)) 
 {
	if(gl_InvocationID == 0) {
		tesselation();
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
