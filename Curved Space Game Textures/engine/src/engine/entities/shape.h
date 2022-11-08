#pragma once

#include "engine/utils/polar_vector.h"

namespace engine
{
	class mesh;

	/// \brief Class creating a cuboid object with a mesh of a specified size
	class shape
	{
	public:
		/// \brief Constructor
		shape(bool loop, polar_vector2_list vertices, float boundingSphere);
		shape(bool loop, polar_vector2_list vertices, float boundingSphere, std::vector<glm::vec2> tex_coordinates);

		/// \brief Destructor
		~shape();

		/// \brief Getter methods
		bool is_loop() const { return s_loop; }
		const polar_vector2_list& vertices() const { return m_vertices; }
		const ref<engine::mesh>& mesh() const { return m_mesh; }
		const float& bounding_sphere() const { return m_bounding_sphere; }
		
		static ref<shape> create(bool loop, polar_vector2_list vertices, float boundingSphere);
		static ref<shape> create(bool loop, polar_vector2_list vertices, float boundingSphere, std::vector<glm::vec2> tex_coordinates);
	private:
		/// \brief Fields
		// Dimensions of the quad specified by width and height. quad is centered on the origin in local coordinates.
		polar_vector2_list m_vertices;

		// Is shape a loop or not
		bool s_loop;

		// Bounding sphere radius
		float m_bounding_sphere;

		ref<engine::mesh> m_mesh;
	};
}
