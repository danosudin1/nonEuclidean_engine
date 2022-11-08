#pragma once
#include "engine/core/timestep.h"
#include "engine/utils/polar_vector.h"

namespace engine
{
	class shape;
	class texture_2d;
	class shader;

    /// \brief Struct used to initialise/create a game object
	struct game_object_properties final
	{
        ref<shape> shape{};
		ref<texture_2d> texture{};
		polar_vector2 position{ 0.f };
		polar_vector2 velocity{ 0.f };
		polar_vector2 acceleration{ 0.f };
		float rotation{ 0.f };
		float angular_velocity{ 0.f };
		float torque{ 0.f };
		std::string type{};
		bool is_static = false;
		float scale = 1.f;
		glm::vec3 colour = glm::vec3(0.f);
		float bounding_circle = 0.f;
	};


	/// \brief Base class for storing the physical information about the objects in the game
	class game_object
    {
    public:
        // TODO - use the following commented ctor only
        game_object(const game_object_properties& props);
        // TODO - lines shouldn't exceed 100 characters, for better readability.
        // TODO - comments in header files should start with "/// \brief ", and there should be one per member/method so that intellisense can show tooltips properly
		/*game_object(const glm::vec3 position, const glm::vec3 velocity, std::vector<ref<mesh>> meshes, const glm::vec3 scale, const bool is_static, float mass, int32_t type, glm::vec3 bounding_shape, glm::vec3 rotation_axis, float rotation_amount);
		game_object(const bool is_static, int32_t type, glm::vec3 bounding_shape);
		game_object(std::vector<ref<mesh>> meshes, int32_t type, glm::vec3 bounding_shape);*/
		/// \brief Destructor -TODO: comments like these are not useful. If the method does something other ppl need to be aware of that's what you should write.
		~game_object();

		/// \brief Getter methods
		polar_vector2 position()		const { return m_position; }
		polar_vector2 velocity()		const { return m_velocity; }
		polar_vector2 acceleration()	const { return m_acceleration; }

		const float& rotation()			const { return m_rotation; }
		const float& angular_velocity()	const { return m_angular_velocity; }
		const float& torque()			const { return m_torque; }
		const float& adjusted_angle()	const { return m_adjusted_angle; }
		const float& initial_angle()	const { return m_initial_angle; }

		const float& scale()			const { return m_scale; }
		const bool& is_static()			const { return s_static; }
		const std::string& type()		const { return m_type; }
		const ref<shape>& shape()		const { return m_shape; }
		const ref<texture_2d>& texture() const { return m_texture; }
		const glm::vec3& colour()		const { return m_colour; }

		const float& bounding_circle()	const { return m_bounding_circle; }

		/// \brief Setter methods
		void set_position(polar_vector2 position) { m_position = position; }
		void set_velocity(polar_vector2 velocity) { m_velocity = velocity; }
		void set_acceleration(polar_vector2 acceleration) { m_acceleration = acceleration; }

		void set_rotation(float rotation) { m_rotation = rotation; }
		void set_angular_velocity(float angular_velocity) { m_angular_velocity = angular_velocity; }
		void set_torque(float torque) { m_torque = torque; }
		void set_initial_angle(float initial_angle) { m_initial_angle = initial_angle; }
		void set_adjusted_angle(float adjusted_angle) { m_adjusted_angle = adjusted_angle; }

		void set_scale(float scale) { m_scale = scale; }
		void set_static_switch(bool is_static) { s_static = is_static; }
		void set_type(std::string type) { m_type = type; }
		void set_shape(ref<engine::shape> shape) { m_shape = shape; }
		void set_texture(ref<texture_2d> texture) { m_texture = texture; }
		void set_colour(glm::vec3 colour) { m_colour = colour; }

		void set_bounding_circle(float bounding_circle) { m_bounding_circle = bounding_circle; }

		// rotate the object to face the specific direction
		void turn_towards(polar_vector2 direction);

		// create a transform matrix using the object's position, rotation and scale member variables
		const void transform(glm::mat4 &object_transform);

		// bind the object's textures if there are any
		void bind_texture();

		void reset();

		void on_update(const timestep& time_step, int screenMaxDist, float K, bool updatePosition);
		void on_update_step(int screenMaxDist, float K, bool updatePosition);

		bool check_collision(const ref<game_object> other_object, float K);

		static float distance(const engine::polar_vector2& position, const engine::polar_vector2& other_position, float K);

    public:
        static ref<game_object> create(const game_object_properties& props);

    private:

		/// \brief Fields
		// object's position vector
		polar_vector2		m_position{ 0.f };
		// object's velocity vector 
		polar_vector2		m_velocity{ 0.f };
		// object's acceleration vector
		polar_vector2		m_acceleration{ 0.f };
		// object's scale
		float				m_scale{ 1.0f };
		// object's current rotation
		float				m_rotation{ 0.f };
		// object's angular velocity
		float				m_angular_velocity{ 0.0f };
		// object's torque
		float				m_torque{ 0.f };
		// object's initial angle
		float				m_initial_angle{ 0.0f };
		// object's adjusted angle
		float				m_adjusted_angle{ 0.0f };
		// object's type
		std::string			m_type;

		// object's mesh
		ref<engine::shape>	m_shape;
		// object's textures
		ref<texture_2d>		m_texture;
		// object's colour
		glm::vec3			m_colour;

		// object's bounding circle
		float				m_bounding_circle{ 0.0f };

        // static/non-static object
		bool				s_static{ false };

		// update the position of the object in the curved coordinate system
		polar_vector2 spherical_position(float scale, float distance);
		polar_vector2 hyperbolic_position(float scale, float distance);
    };
}
