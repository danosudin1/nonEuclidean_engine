#include "pch.h"
#include "polygon_equation.h"
#include "engine/renderer/mesh.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Default constructor. */
engine::polygon_equation::polygon_equation(void)
{}

/** Destructor. */
engine::polygon_equation::~polygon_equation(void)
{}

/** Construct the list of points for a polygon depending on the number of vertices and number of extra points on each edge. */
engine::polygon_equation::polygon_equation(float radius, float vertex_number) : polygon(radius, vertex_number)
{}

engine::polygon_equation::polygon_equation(std::vector<engine::mesh::vertex> polygon_vertices, int tesselation_number, float K, float rotation_angle, polar_vector2 object_position, bool is_loop, bool great_circle_navigation)
{
	vertices_vector = polygon_vertices;
	if (K > 0.01 || K < -0.01) {
		float k;
		if (K > 0) {
			k = 160.0f / sqrt(K);
		}
		else {
			k = 160.0f / sqrt(-1.0f * K);
		}

		if (object_position.r == 90.f)
		{
			int i = 0.f;
		}

		curvature_vertices(k, K, rotation_angle, object_position);

		if (tesselation_number > 0) {
			add_equation_tesselation(tesselation_number, k, is_loop, K, object_position, great_circle_navigation);
		}
		else {
			for (int i = 0; i < vertex_vs.size(); i++) {
				points_list.push_back(polar_vector2(vertex_vs[i].r * k, vertex_vs[i].theta));
			}
		}
	}

}

void engine::polygon_equation::add_equation_tesselation(int tesselation_number, float k, bool isLoop, float K, polar_vector2 object_position, bool great_circle_navigation) {
	if (K <= 0.01f && K >= -0.01f) {
		polar_vector2 other_point = polar_vector2(vertices_vector.back().position.x, vertices_vector.back().position.y);
		for (auto vertex : vertices_vector) {

			polar_vector2 current_point = polar_vector2(vertex.position.x, vertex.position.y);

			for (int i = 1; i < (tesselation_number + 1); i++) {

				float coefficient_1 = i / ((float)tesselation_number + 1.0f);
				float coefficient_2 = 1 - coefficient_1;
				polar_vector2 point = (current_point * coefficient_1) + (other_point * coefficient_2);

				points_list.push_back(point);
			}

			other_point = current_point;
			points_list.push_back(other_point);
		}
	}
	else {
		float r = 0.0;
		for (int i = 0; i < vertex_vs.size() - 1; i++) {
			points_list.push_back(polar_vector2(vertex_vs[i].r * k, vertex_vs[i].theta));

			if (K > 0.01f) spherical_equation_tesselation(i, i + 1, tesselation_number, k, object_position, great_circle_navigation);
			else hyperbolic_equation_tesselation(i, i + 1, tesselation_number, k, object_position);
		}

		points_list.push_back(polar_vector2(vertex_vs[vertex_vs.size() - 1].r * k, vertex_vs[vertex_vs.size() - 1].theta));

		if (isLoop) {
			if (K > 0.01f) spherical_equation_tesselation(vertex_vs.size() - 1, 0, tesselation_number, k, object_position, great_circle_navigation);
			else hyperbolic_equation_tesselation(vertex_vs.size() - 1, 0, tesselation_number, k, object_position);
		}
	}
}

void engine::polygon_equation::spherical_equation_tesselation(int vertex_1, int vertex_2, int tesselation_number, float k, polar_vector2 object_position, bool great_circle_navigation) {
	if (great_circle_navigation)
	{
		float delta_theta = abs(vertex_vs[vertex_1].theta - vertex_vs[vertex_2].theta);

		if (!(delta_theta == 0.0f || vertex_vs[vertex_1].r == 0.f || vertex_vs[vertex_2].r == 0.f)) {
			// convert to latitude / longitude
			// long = mod(theta + 180, 360) - 180
			// lat = 90 - r

			float phi_1 = m_pi / 2.f - vertex_vs[vertex_1].r;
			float phi_2 = m_pi / 2.f - vertex_vs[vertex_2].r;
			float lambda_1 = fmod(vertex_vs[vertex_1].theta + m_pi, 2.f * m_pi) - m_pi;
			float lambda_2 = fmod(vertex_vs[vertex_2].theta + m_pi, 2.f * m_pi) - m_pi;

			float sin_phi_1 = sin(phi_1);
			float cos_phi_1 = cos(phi_1);
			float sin_phi_2 = sin(phi_2);
			float cos_phi_2 = cos(phi_2);

			// calculate the preliminaries
			float lambda_12 = lambda_2 - lambda_1;
			if (lambda_12 < -m_pi)
				lambda_12 = 2.f * m_pi + lambda_12;
			if (lambda_12 > m_pi)
				lambda_12 = lambda_12 - 2.f * m_pi;

			float alpha_1 = atan2(cos_phi_2 * sin(lambda_12), cos_phi_1 * sin_phi_2 - sin_phi_1 * cos_phi_2 * cos(lambda_12));
			float sin_alpha_1 = sin(alpha_1);
			float cos_alpha_1 = cos(alpha_1);

			float alpha_0 = atan2(sin_alpha_1 * cos_phi_1, sqrt(cos_alpha_1 * cos_alpha_1 + sin_alpha_1 * sin_alpha_1 * sin_phi_1 * sin_phi_1));
			float cot_alpha_0 = 1 / tan(alpha_0);

			float tan_phi_1;

			int i = 0;
			//else
			tan_phi_1 = tan(phi_1);

			float sigma_01 = atan2(tan_phi_1, cos(alpha_1));

			float lambda_01 = atan2(sin(alpha_0) * sin(sigma_01), cos(sigma_01));

			float lambda_0 = lambda_1 - lambda_01;


			// for loop
			//   calculate new lat/long
			//   convert back to r/theta

			float increment = lambda_12 / tesselation_number;

			for (int i = 1; i < tesselation_number; i++)
			{
				float lambda_i = lambda_1 + increment * i;

				float phi_i = atan(cot_alpha_0 * sin(lambda_i - lambda_0));

				float new_r = m_pi / 2.f - phi_i;
				float new_theta = polar_vector2::constrain_angle_rad(lambda_i);

				polar_vector2 new_coordinate = polar_vector2(k * new_r, new_theta);

				points_list.push_back(new_coordinate);
			}
		}
	}
	else
	{
		// colatitude of the vertex is it's normalised r coordinate and we recorded the normalised r coordinate in the previous step
		// find cartesian coordinates on a unit sphere
		// x = cos(longitude) * sin(colatitude);
		// y = sin(longitude) * sin(colatitude);
		// z = cos (colatitude)

		glm::vec3 vertex_1_cartesian = glm::vec3(cos(vertex_vs[vertex_1].theta) * sin(vertex_vs[vertex_1].r), sin(vertex_vs[vertex_1].theta) * sin(vertex_vs[vertex_1].r), cos(vertex_vs[vertex_1].r));
		glm::vec3 vertex_2_cartesian = glm::vec3(cos(vertex_vs[vertex_2].theta) * sin(vertex_vs[vertex_2].r), sin(vertex_vs[vertex_2].theta) * sin(vertex_vs[vertex_2].r), cos(vertex_vs[vertex_2].r));

		// find u and v - orthogonalvectors lying in the plane of the great circle going through the vertices 1 and 2

		glm::vec3 u = glm::normalize(vertex_1_cartesian);
		glm::vec3 w = glm::normalize(glm::cross(vertex_1_cartesian, vertex_2_cartesian));
		glm::vec3 v = glm::cross(u, w);

		// circle equation is c = r(u cos(omega) + v sin(omega)) when 0 < omega < 360. As we are dealing with a unit sphere, r = 1
		// when omega = 0, c = vertex_1
		// find the angle between vertex_1 and vertex_2

		float delta_omega = acos(glm::dot(vertex_1_cartesian, vertex_2_cartesian) / ((glm::length(vertex_1_cartesian)) * (glm::length(vertex_2_cartesian))));
		if (delta_omega > m_pi) delta_omega -= 2.f * m_pi;
		if (delta_omega < -m_pi) delta_omega += 2.f * m_pi;

		float omega_increment = -1.f * delta_omega / tesselation_number;

		for (int i = 1; i < tesselation_number; i++)
		{
			//omega -= delta_omega * 0.05f;
			glm::vec3 new_coordinate_cartesian = u * cos(omega_increment * i) + v * sin(omega_increment * i);

			polar_vector2 new_coordinate = polar_vector2(k * acos(new_coordinate_cartesian.z), atan2f(new_coordinate_cartesian.y, new_coordinate_cartesian.x));

			points_list.push_back(new_coordinate);
		}
	}
}

void engine::polygon_equation::hyperbolic_equation_tesselation(int vertex_1, int vertex_2, int tesselation_number, float k, polar_vector2 object_position) {
	if (!(vertex_vs[vertex_1].r == 0.f || vertex_vs[vertex_2].r == 0.f)) {
		float r_poincare_vertex_1 = tanh(vertex_vs[vertex_1].r / 2.f);
		float r_poincare_vertex_2 = tanh(vertex_vs[vertex_2].r / 2.f);

		float v_x_global = r_poincare_vertex_1 * cos(vertex_vs[vertex_1].theta);
		float v_y_global = r_poincare_vertex_1 * sin(vertex_vs[vertex_1].theta);
		float u_x_global = r_poincare_vertex_2 * cos(vertex_vs[vertex_2].theta);
		float u_y_global = r_poincare_vertex_2 * sin(vertex_vs[vertex_2].theta);

		// find poincare geodesic circle equation. x^2 + y^2 + ax + by + 1 = 0 

		float a = (u_y_global * (v_x_global * v_x_global + v_y_global * v_y_global) - v_y_global * (u_x_global * u_x_global + u_y_global * u_y_global)
			+ u_y_global - v_y_global) / (u_x_global * v_y_global - u_y_global * v_x_global);

		float b = (v_x_global * (u_x_global * u_x_global + u_y_global * u_y_global) - u_x_global * (v_x_global * v_x_global + v_y_global * v_y_global)
			+ v_x_global - u_x_global) / (u_x_global * v_y_global - u_y_global * v_x_global);

		// Find centre of the circle and radius. As well as the distance (centre_r coordinate) to the centre from origin

		float radius = sqrt(0.25f * a * a + 0.25f * b * b - 1);
		float centre_x = -0.5f * a;
		float centre_y = -0.5f * b;
		float centre_theta = atan2f(centre_y, centre_x);
		float centre_r = sqrt(centre_x * centre_x + centre_y * centre_y);

		// Find local theta coordinate of point p, v and u within the circle

		float v_theta_local = atan2f(v_y_global - centre_y, v_x_global - centre_x);
		float u_theta_local = atan2f(u_y_global - centre_y, u_x_global - centre_x);
		// u_r_local, v_r_local is radius

		// ... TODO

		float u_v_dif = u_theta_local - v_theta_local;
		if (u_v_dif > m_pi) u_v_dif -= 2.f * m_pi;
		if (u_v_dif < -m_pi) u_v_dif += 2.f * m_pi;
		float increment = (u_v_dif) / tesselation_number;
		for (int i = 0; i < tesselation_number; i++)
		{
			polar_vector2 new_coordinate_poincare = polar_vector2(radius, v_theta_local + increment * i) + polar_vector2(centre_r, centre_theta);

			polar_vector2 new_coordinate = polar_vector2(k * 2.f * atanh(new_coordinate_poincare.r), new_coordinate_poincare.theta);
			//polar_vector2 new_coordinate = polar_vector2(160.0f * 2.f new_coordinate_poincare.r, new_coordinate_poincare.theta); //<- poincare disk projection

			points_list.push_back(new_coordinate);
		}
	}
	/*for (int i = -20; i < 21; i++) {
		float new_local_angle = 0.157f * i;
		polar_vector2 new_coordinate_poincare = polar_vector2(radius, new_local_angle + p_theta_local) + polar_vector2(centre_r, centre_theta);

		polar_vector2 new_coordinate = polar_vector2(160.0f * 2.f * atanh(new_coordinate_poincare.r) , new_coordinate_poincare.theta);

		points_list.push_back(new_coordinate);
	}*/
	
	
	/*

	float delta_theta = abs(vertex_vs[vertex_1].theta - vertex_vs[vertex_2].theta);

	if (delta_theta > m_pi) delta_theta = 2 * m_pi - delta_theta;
	
	if (!(delta_theta == 0.0f || vertex_vs[vertex_1].r == 0.f || vertex_vs[vertex_2].r == 0.f)) {
		float cos_delta_theta = cos(delta_theta);

		float cosh_edge = vertex_vs[vertex_1].cosh_r * vertex_vs[vertex_2].cosh_r - vertex_vs[vertex_1].sinh_r * vertex_vs[vertex_2].sinh_r * cos_delta_theta;
		float edge = acosh(cosh_edge);
		float sinh_edge = sinh(edge);

		float cos_alpha = ((vertex_vs[vertex_1].cosh_r * cosh_edge) - vertex_vs[vertex_2].cosh_r) / (vertex_vs[vertex_1].sinh_r * sinh_edge);

		float sin_alpha = sin(acos(cos_alpha));

		float sinh_height =  vertex_vs[vertex_1].sinh_r * sin_alpha;

		float height = asinh(sinh_height);

		float sin_delta_height = cos_alpha / cosh(height);

		float delta_theta_height = asin(sin_delta_height);

		int direction = 1;

		if (vertex_vs[vertex_2].theta < vertex_vs[vertex_1].theta && !(vertex_vs[vertex_1].delta_theta == 0.0f && vertex_vs[vertex_2].delta_theta == 0.0f)
			&& !((vertex_vs[vertex_1].delta_theta + vertex_vs[vertex_2].delta_theta) > m_pi)) direction = -1;

		points_list.push_back(polar_vector2(height * k, vertex_vs[vertex_1].theta + direction * delta_theta_height));
	
	
		float max = vertex_vs[vertex_1].r;
		if (vertex_vs[vertex_2].r > vertex_vs[vertex_1].r) max = vertex_vs[vertex_2].r;
		max = max * k / 160.0f;

		int modified_tesselation = (int)(max * 10.0f * (float)tesselation_number);

		float part_edge = edge / (modified_tesselation + 1);

		/*int direction = 1;

		if (vertex_vs[vertex_2].theta < vertex_vs[vertex_1].theta && !(vertex_vs[vertex_1].delta_theta == 0.0f && vertex_vs[vertex_2].delta_theta == 0.0f)
			&& !((vertex_vs[vertex_1].delta_theta + vertex_vs[vertex_2].delta_theta) > m_pi)) direction = -1;*/


	/*
		for (int j = 1; j < (modified_tesselation + 1); j++) {
			float d = part_edge * j;
			float cosh_d = cosh(d);
			float sinh_d = sinh(d);

			float cosh_r_v = vertex_vs[vertex_1].cosh_r * cosh_d - vertex_vs[vertex_1].sinh_r * sinh_d * cos_alpha;
			float r_v = acosh(cosh_r_v);
			float sinh_r_v = sinh(r_v);

			float cos_delta_theta_v = ((vertex_vs[vertex_1].cosh_r * cosh_r_v) - cosh_d) / (vertex_vs[vertex_1].sinh_r * sinh_r_v);
			if (cos_delta_theta_v > 1.0f) cos_delta_theta_v = 1.0f;
			if (cos_delta_theta_v < -1.0f) cos_delta_theta_v = -1.0f;
			float delta_theta_v = acos(cos_delta_theta_v);

			float theta_v = vertex_vs[vertex_1].theta + direction * delta_theta_v;

			float r = r_v * k;
			points_list.push_back(polar_vector2(r, theta_v));
		}
		
	}
	*/
}
