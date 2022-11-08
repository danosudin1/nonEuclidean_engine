#pragma once
#include "engine/utils/polar_vector.h"
#include "engine/renderer/mesh.h"
#include "engine/entities/polygon.h"

namespace engine
{
	/*
	struct vertex_variables final
	{
		float r{ 0.f };
		float theta{ 0.f };
		int direction{ 0 };
		float cos_r{ 0.f };
		float sin_r{ 0.f };
		float cosh_r{ 0.f };
		float sinh_r{ 0.f };
		float delta_theta{ 0.f };
		float cos_delta_theta{ 0.f };
	};*/

	class polygon_equation : public polygon
	{
	public:
		polygon_equation(void);
		~polygon_equation(void);
		polygon_equation(float radius, float vertex_number);
		polygon_equation(std::vector<mesh::vertex> polygon_vertices, int tesselation_number, float K, float rotation_angle, polar_vector2 object_position, bool is_loop, bool great_circle_navigation);

		void add_equation_tesselation(int tesselation_number, float k, bool is_loop, float K, polar_vector2 object_position, bool great_circle_navigation);

		void spherical_equation_tesselation(int point_1_index, int point_2_index, int tesselation_number, float k, polar_vector2 object_position, bool great_circle_navigation);
		void hyperbolic_equation_tesselation(int point_1_index, int point_2_index, int tesselation_number, float k, polar_vector2 object_position);

	private:
		/*polar_vector2_list points_list;
		std::vector<mesh::vertex> vertices_vector;

		// explained in the model description document
		// vertexVariables:
		// [0][j] vertex 1
		// [1][j] vertex 2
		// ...
		// [n][j] vertex n+1
		//
		// [i][0] R
		// [i][1] Theta
		// [i][2] direction
		// [i][3] cosR (cosh for Hyperbolic)
		// [i][4] sinR (sinh for Hyperbolic)
		// [i][5] DeltaTheta
		// [i][6] cosDeltaTheta
		// [i][7] scaleToUnit

		std::vector<std::vector<float>> vertexVariables;
		std::vector<vertex_variables> vertex_vs;*/
	};
}

