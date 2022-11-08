#include "pch.h"
#include "polygon.h"
#include "engine/renderer/mesh.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Default constructor. */
engine::polygon::polygon(void)
{}

/** Destructor. */
engine::polygon::~polygon(void)
{}

/** Construct the list of points for a polygon depending on the number of vertices and number of extra points on each edge. */
engine::polygon::polygon(float radius, float vertex_number)
{
	float deg_increment = 360.0f / vertex_number;
	for (int i = 0; i < vertex_number; i++) {
		points_list.push_back(polar_vector2(radius, DEG2RAD * (deg_increment * i + deg_increment / 2)));
	}
}

engine::polygon::polygon(std::vector<engine::mesh::vertex> polygon_vertices, int tesselation_number, float K, float rotation_angle, polar_vector2 object_position, bool is_loop, bool non_curved_tesselation)
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
			add_tesselation(tesselation_number, k, is_loop, K, object_position);
		}
		else {
			for (int i = 0; i < vertex_vs.size(); i++) {
				points_list.push_back(polar_vector2(vertex_vs[i].r * k, vertex_vs[i].theta));
			}
		}
	}
	else {
		// non-curved tesselation
		if (non_curved_tesselation)
			add_non_curved_tesselation(tesselation_number, is_loop, object_position);
	}

}

void engine::polygon::curvature_vertices(float k, float K, float rotation_angle, polar_vector2 object_position) {

	//= abs(((float)maxDistance) / (K * 2 * (float)M_PI));

	for (auto vertex : vertices_vector) {
		polar_vector2 current_point = polar_vector2(vertex.position.x, vertex.position.y);

		engine::vertex_variables current_vertex_vs;

		if (K > 0) {
			current_vertex_vs = spherical_vertices(k, rotation_angle, object_position, current_point);
		}
		else {
			current_vertex_vs = hyperbolic_vertices(k, rotation_angle, object_position, current_point);
		}
		vertex_vs.push_back(current_vertex_vs);
	}
}

void engine::polygon::add_tesselation(int tesselation_number, float k, bool isLoop, float K, polar_vector2 object_position) {
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

			if (K > 0.01f) spherical_tesselation(i, i + 1, tesselation_number, k, object_position);
			else hyperbolic_tesselation(i, i + 1, tesselation_number, k, object_position);
		}

		points_list.push_back(polar_vector2(vertex_vs[vertex_vs.size() - 1].r * k, vertex_vs[vertex_vs.size() - 1].theta));

		if (isLoop) {
			if (K > 0.01f) spherical_tesselation(vertex_vs.size() - 1, 0, tesselation_number, k, object_position);
			else hyperbolic_tesselation(vertex_vs.size() - 1, 0, tesselation_number, k, object_position);
		}
	}
}

engine::vertex_variables engine::polygon::spherical_vertices(float k, float rotation_angle, polar_vector2 object_position, polar_vector2 point) {

	vertex_variables variables;

	if (object_position.r == 0.0f) {
		float r = point.r / k;
		variables.r = r;
		variables.theta = engine::polar_vector2::constrain_angle_rad(point.theta + rotation_angle);
		variables.direction = 1;
		variables.cos_r = cos(r);
		variables.sin_r = sin(r);
		variables.delta_theta = 0.0f;
		variables.cos_delta_theta = 0.0f;

		return variables;
	}

	float beta = engine::polar_vector2::constrain_angle_rad(-(point.theta + rotation_angle));
	if (((beta < 0.0001f || abs(beta - 2.f*m_pi) < 0.0001f) && object_position.r - point.r == 0.f) || (abs(beta - m_pi) < 0.0001f && object_position.r + point.r == 0.f)) {
		variables.r = 0.f;
		variables.theta = engine::polar_vector2::constrain_angle_rad(point.theta + rotation_angle);
		variables.direction = 1;
		variables.cos_r = cos(0.f);
		variables.sin_r = sin(0.f);
		variables.delta_theta = 0.f;
		variables.cos_delta_theta = 0.f;

		return variables;
	}

	float r_c = object_position.r / k;
	float cos_r_c = cos(r_c);
	float sin_r_c = sin(r_c);

	int direction = 1;
	if (beta > float(m_pi)) { beta = float(2 * m_pi) - beta; direction *= -1; }
	float cos_beta = cos(beta);

	float r_local = point.r / k;
	float cos_r_local = cos(r_local);
	float sin_r_local = sin(r_local);

	//using Spherical Law of Cosines
	float cos_r_v = cos_r_c * cos_r_local + sin_r_c * sin_r_local * cos_beta;
	float r_v = acos(cos_r_v);
	float sin_r_v = sin(r_v);

	float cos_delta_theta = (cos_r_local - (cos_r_c * cos_r_v)) / (sin_r_c * sin_r_v);
	if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
	if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;
	float delta_theta = acos(cos_delta_theta);
	float theta_v = object_position.theta + direction * delta_theta;

	variables.r = r_v;
	variables.theta = theta_v;
	variables.direction = direction;
	variables.cos_r = cos_r_v;
	variables.sin_r = sin_r_v;
	variables.delta_theta = delta_theta;
	variables.cos_delta_theta = cos_delta_theta;

	return variables;
}

engine::vertex_variables engine::polygon::hyperbolic_vertices(float k, float rotation_angle, polar_vector2 object_position, polar_vector2 point) {

	vertex_variables variables;

	if (object_position.r == 0.0f) {
		float r = point.r / k;
		variables.r = r;
		variables.theta = engine::polar_vector2::constrain_angle_rad(point.theta + rotation_angle);
		variables.direction = 1;
		variables.cosh_r = cosh(r);
		variables.sinh_r = sinh(r);
		variables.delta_theta = 0.0f;
		variables.cos_delta_theta = 0.0f;

		return variables;
	}

	float beta = engine::polar_vector2::constrain_angle_rad(-(point.theta + rotation_angle));
	if (((beta < 0.0001f || abs(beta - 2.f * m_pi) < 0.0001f) && object_position.r - point.r == 0.f) || (abs(beta - m_pi) < 0.0001f && object_position.r + point.r == 0.f)) {
		variables.r = 0.f;
		variables.theta = engine::polar_vector2::constrain_angle_rad(point.theta + rotation_angle);
		variables.direction = 1;
		variables.cosh_r = cosh(0.f);
		variables.sinh_r = sinh(0.f);
		variables.delta_theta = 0.f;
		variables.cos_delta_theta = 0.f;

		return variables;
	}

	float r_c = object_position.r / k;
	float cosh_r_c = cosh(r_c);
	float sinh_r_c = sinh(r_c);

	int direction = 1;
	if (beta > float(m_pi)) { beta = float(2 * m_pi) - beta; direction *= -1; }
	float cos_beta = cos(beta);

	float r_local = point.r / k;
	float cosh_r_local = cosh(r_local);
	float sinh_r_local = sinh(r_local);

	//using Hyperbolic Law of Cosines
	float cosh_r_v = cosh_r_c * cosh_r_local - sinh_r_c * sinh_r_local * cos_beta;
	float r_v = acosh(cosh_r_v);
	float sinh_r_v = sinh(r_v);

	float cos_delta_theta = ((cosh_r_c * cosh_r_v) - cosh_r_local) / (sinh_r_c * sinh_r_v);
	if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
	if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;
	float delta_theta = acos(cos_delta_theta);
	float theta = object_position.theta + direction * delta_theta;

	variables.r = r_v;
	variables.theta = theta;
	variables.direction = direction;
	variables.cosh_r = cosh_r_v;
	variables.sinh_r = sinh_r_v;
	variables.delta_theta = delta_theta;
	variables.cos_delta_theta = cos_delta_theta;

	return variables;
}

void engine::polygon::spherical_tesselation(int vertex_1, int vertex_2, int tesselation_number, float k, polar_vector2 object_position) {
	float delta_theta = abs(vertex_vs[vertex_1].theta - vertex_vs[vertex_2].theta);

	if (!(delta_theta == 0.0f || vertex_vs[vertex_1].r == 0.f || vertex_vs[vertex_2].r == 0.f)) {
		float cos_delta_theta = cos(delta_theta);

		float cos_d = vertex_vs[vertex_1].cos_r * vertex_vs[vertex_2].cos_r + vertex_vs[vertex_1].sin_r * vertex_vs[vertex_2].sin_r * cos_delta_theta;
		float d = acos(cos_d);
		float sin_d = sin(d);

		float cos_alpha = (vertex_vs[vertex_2].cos_r - (vertex_vs[vertex_1].cos_r * cos_d)) / (vertex_vs[vertex_1].sin_r * sin_d);

		float max = vertex_vs[vertex_1].r;
		if (vertex_vs[vertex_2].r > vertex_vs[vertex_1].r) max = vertex_vs[vertex_2].r;
		max = max * k / 160.f;

		//int modified_tesselation = (int)(max * 10.0f * (float)tesselation_number);

		float part_edge = d / tesselation_number; // d / (modified_tesselation +1)

		int direction = 1;

		float dif_theta_1 = abs(object_position.theta - vertex_vs[vertex_1].theta);
		float dif_theta_2 = abs(object_position.theta - vertex_vs[vertex_2].theta);

		if (vertex_vs[vertex_2].theta < vertex_vs[vertex_1].theta && ((dif_theta_1 + dif_theta_2) < m_pi)) direction = -1;
		if (((dif_theta_1 + dif_theta_2) > m_pi) && object_position.r > 200) direction = -1;

		for (int j = 1; j < tesselation_number; j++) {        //for (int j = 1; j < (modified_tesselation +1); j++)
			float d_i = part_edge * j;

			float cos_d_i = cos(d_i);
			float sin_d_i = sin(d_i);

			float cos_r_i = vertex_vs[vertex_1].cos_r * cos_d_i + vertex_vs[vertex_1].sin_r * sin_d_i * cos_alpha;
			float r_i = acos(cos_r_i);
			float sin_r_i = sin(r_i);

			float cos_delta_theta_i = (cos_d_i - (vertex_vs[vertex_1].cos_r * cos_r_i)) / (vertex_vs[vertex_1].sin_r * sin_r_i);
			if (cos_delta_theta_i > 1.0f) cos_delta_theta_i = 1.0f;
			if (cos_delta_theta_i < -1.0f) cos_delta_theta_i = -1.0f;
			float delta_theta_i = acos(cos_delta_theta_i);


			float theta_i = vertex_vs[vertex_1].theta + direction * delta_theta_i;

			float r = r_i * k;
			points_list.push_back(polar_vector2(r, theta_i));
		}
	}
}

void engine::polygon::hyperbolic_tesselation(int vertex_1, int vertex_2, int tesselation_number, float k, polar_vector2 object_position) {
	float delta_theta = abs(vertex_vs[vertex_1].theta - vertex_vs[vertex_2].theta);

	if (delta_theta > m_pi) delta_theta = 2 * m_pi - delta_theta;
	
	if (!(delta_theta == 0.0f || vertex_vs[vertex_1].r == 0.f || vertex_vs[vertex_2].r == 0.f)) {
		float cos_delta_theta = cos(delta_theta);

		float cosh_d = vertex_vs[vertex_1].cosh_r * vertex_vs[vertex_2].cosh_r - vertex_vs[vertex_1].sinh_r * vertex_vs[vertex_2].sinh_r * cos_delta_theta;
		float d = acosh(cosh_d);
		float sinh_d = sinh(d);

		float cos_alpha = ((vertex_vs[vertex_1].cosh_r * cosh_d) - vertex_vs[vertex_2].cosh_r) / (vertex_vs[vertex_1].sinh_r * sinh_d);

		float max = vertex_vs[vertex_1].r;
		if (vertex_vs[vertex_2].r > vertex_vs[vertex_1].r) max = vertex_vs[vertex_2].r;
		max = max * k / 160.0f;

		//int modified_tesselation = (int)(max * 10.0f * (float)tesselation_number);

		float part_edge = d / tesselation_number;//d / (modified_tesselation + 1);

		int direction = 1;

		if (vertex_vs[vertex_2].theta < vertex_vs[vertex_1].theta && !(vertex_vs[vertex_1].delta_theta == 0.0f && vertex_vs[vertex_2].delta_theta == 0.0f)
			&& !((vertex_vs[vertex_1].delta_theta + vertex_vs[vertex_2].delta_theta) > m_pi)) direction = -1;

		for (int j = 1; j < tesselation_number; j++) {         // for (int j = 1; j < (modified_tesselation + 1); j++) 
			float d_i = part_edge * j;
			float cosh_d_i = cosh(d_i);
			float sinh_d_i = sinh(d_i);

			float cosh_r_i = vertex_vs[vertex_1].cosh_r * cosh_d_i - vertex_vs[vertex_1].sinh_r * sinh_d_i * cos_alpha;
			float r_i = acosh(cosh_r_i);
			float sinh_r_i = sinh(r_i);

			float cos_delta_theta_i = ((vertex_vs[vertex_1].cosh_r * cosh_r_i) - cosh_d_i) / (vertex_vs[vertex_1].sinh_r * sinh_r_i);
			if (cos_delta_theta_i > 1.0f) cos_delta_theta_i = 1.0f;
			if (cos_delta_theta_i < -1.0f) cos_delta_theta_i = -1.0f;
			float delta_theta_i = acos(cos_delta_theta_i);

			float theta_i = vertex_vs[vertex_1].theta + direction * delta_theta_i;

			float r = r_i * k;
			points_list.push_back(polar_vector2(r, theta_i));
		}
	}
}


void engine::polygon::add_non_curved_tesselation(int tesselation_number, bool isLoop, polar_vector2 object_position) {
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
