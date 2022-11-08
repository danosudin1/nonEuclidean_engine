#include "pch.h"
#include "shape.h"
#include <engine.h>

engine::shape::shape(bool loop, engine::polar_vector2_list vertices, float bounding_sphere) : s_loop(loop), m_vertices(vertices), m_bounding_sphere(bounding_sphere)
{
	std::vector<mesh::vertex> shape_vertices;
	std::vector<uint32_t> shape_indices;
	unsigned int count = 0;

	float max_r = 0.f;
	for (polar_vector2_list::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {
		if (it->r > max_r)
			max_r = it->r;
	}
	for (polar_vector2_list::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {
		mesh::vertex vertex;
		vertex.position = glm::vec3(it->r, it->theta, 0.f);
		vertex.normal = glm::vec3(0.f, 0.f, 1.f);
		polar_vector2 normalised = polar_vector2(it->r / max_r, it->theta);
		glm::vec3 cartesian_it_normalised = normalised.ToCartesian()/2.f;
		vertex.tex_coords = glm::vec2(0.5f, 0.5f) + glm::vec2(cartesian_it_normalised.x, cartesian_it_normalised.y);

		shape_vertices.push_back(vertex);

		shape_indices.push_back(count);
		if (count < m_vertices.size() - 1) {
			shape_indices.push_back(count + 1);
		}
		else {
			if (s_loop) shape_indices.push_back(0);
		}
		count++;
	}

	m_mesh = engine::mesh::create(shape_vertices, shape_indices);
}

engine::shape::shape(bool loop, engine::polar_vector2_list vertices, float bounding_sphere, std::vector<glm::vec2> tex_coordinates) : s_loop(loop), m_vertices(vertices), m_bounding_sphere(bounding_sphere)
{
	std::vector<mesh::vertex> shape_vertices;
	std::vector<uint32_t> shape_indices;
	unsigned int count = 0;

	for (polar_vector2_list::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {

		mesh::vertex vertex;
		vertex.position = glm::vec3(it->r, it->theta, 0.f);
		vertex.normal = glm::vec3(0.f, 0.f, 1.f);
		vertex.tex_coords = tex_coordinates[count];

		shape_vertices.push_back(vertex);

		shape_indices.push_back(count);
		if (count < m_vertices.size() - 1) {
			shape_indices.push_back(count + 1);
		}
		else {
			if (s_loop) shape_indices.push_back(0);
		}
		count++;
	}


	

	m_mesh = engine::mesh::create(shape_vertices, shape_indices);
}

engine::shape::~shape() {}

engine::ref<engine::shape> engine::shape::create(bool loop, engine::polar_vector2_list vertices, float bounding_sphere)
{
	return std::make_shared<engine::shape>(loop, vertices, bounding_sphere);
}

engine::ref<engine::shape> engine::shape::create(bool loop, engine::polar_vector2_list vertices, float bounding_sphere, std::vector<glm::vec2> tex_coordinates)
{
	return std::make_shared<engine::shape>(loop, vertices, bounding_sphere, tex_coordinates);
}
