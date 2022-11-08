#version 400 core

#define m_pi 3.1415926535897932384626433832795

uniform int num_strips;
uniform int num_segments;

layout( vertices=2 ) out;

uniform vec3 in_object_position;
uniform float in_k;

out float delta_omega[];

out vec3 u[];
out vec3 v[];

float theta_v0;
float theta_v1;
float r_v0;
float r_v1;

void tesselation()
{
	
	vec3 vertex_1_cartesian = vec3(cos(theta_v0) * sin(r_v0), sin(theta_v0) * sin(r_v0), cos(r_v0));
	vec3 vertex_2_cartesian = vec3(cos(theta_v1) * sin(r_v1), sin(theta_v1) * sin(r_v1), cos(r_v1));

	// find u and v - orthogonalvectors lying in the plane of the great circle going through the vertices 1 and 2

	u[gl_InvocationID] = normalize(vertex_1_cartesian);
	vec3 w = normalize(cross(vertex_1_cartesian, vertex_2_cartesian));
	v[gl_InvocationID] = cross(u[0], w);

	// circle equation is c = r(u cos(omega) + v sin(omega)) when 0 < omega < 360. As we are dealing with a unit sphere, r = 1
	// when omega = 0, c = vertex_1
	// find the angle between vertex_1 and vertex_2

	delta_omega[gl_InvocationID] = acos(dot(vertex_1_cartesian, vertex_2_cartesian) / ((length(vertex_1_cartesian)) * (length(vertex_2_cartesian))));
	if (delta_omega[gl_InvocationID] > m_pi) delta_omega[gl_InvocationID] -= 2.f * m_pi;
	if (delta_omega[gl_InvocationID] < -m_pi) delta_omega[gl_InvocationID] += 2.f * m_pi;

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
