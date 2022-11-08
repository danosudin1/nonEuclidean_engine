#include "asteroids_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "../game_world.h"

asteroids_layer::asteroids_layer(engine::ref<game_world> game_world)
    : m_game_world(game_world) {}

asteroids_layer::~asteroids_layer() {}

void asteroids_layer::on_update(const engine::timestep& time_step)
{
	
	if (m_game_world->get_game_state() == game_world::game_state::running)
	{
		frame_timer += (float)time_step;

		m_game_world->update_timer((float)time_step);

		// update the spaceship
		m_game_world->get_spaceship()->on_update(time_step, (float)engine::application::window().width() / 2.f, m_game_world->get_K(), true);

		// update the bullets
		m_game_world->get_bullets()->on_update(time_step, (float)engine::application::window().width() / 2.f, m_game_world->get_K(), true);

		// update the asteroids
		m_game_world->get_asteroids()->on_update(time_step, (float)engine::application::window().width() / 2.f, m_game_world->get_K(), true);

		m_game_world->get_asteroids()->check_collision(m_game_world->get_bullets()->get_bullets(), m_game_world->score());

		if(m_game_world->get_spaceship()->check_collision(m_game_world->get_asteroids()->objects()) || m_game_world->get_asteroids()->objects().size() == 0)
		{
			m_game_world->get_spaceship()->object()->set_velocity(engine::polar_vector2(0.f, 0.f));
			m_game_world->set_game_state(game_world::game_state::stopped);
		}
	}
} 

void asteroids_layer::on_render()
{

	// current world curvature
	float K = m_game_world->get_K();

	// Set up non_curved shader to render the axis
	engine::ref<engine::shader> non_curved_shader = engine::renderer::shaders_library()->get("non_curvature_shader");

	

	// Set up curved shader. (does not render textures, renders curved lines instead)
	engine::ref<engine::shader> shader;
	engine::ref<engine::shader> shader_lines;
	if (K > 0.01f)
	{
		shader = engine::renderer::shaders_library()->get("curvature_shader_spherical");
		shader_lines = engine::renderer::shaders_library()->get("curvature_shader_spherical_lines");
	}
	else if (K < -0.01f)
	{
		shader = engine::renderer::shaders_library()->get("curvature_shader_hyperbolic");
		shader_lines = engine::renderer::shaders_library()->get("curvature_shader_hyperbolic_lines");
	}
	else
		shader = engine::renderer::shaders_library()->get("non_curved_tesselation_shader");
		

	
	if (K > 0.01f || K < -0.01f)
	{
		engine::renderer::begin_scene(m_game_world->camera(), shader_lines);
		// render the bullets
		for (auto bullet : m_game_world->get_bullets()->get_bullets())
			engine::renderer::submit_curved_lines(shader_lines, bullet->object(), m_game_world->tess_parameter(), K);

		// render the rest of the grid
		for (uint32_t i = 2; i < m_game_world->grid().size(); i++)
			engine::renderer::submit_curved_lines(shader_lines, m_game_world->grid().at(i), m_game_world->tess_parameter(), K);
	}
	else
	{
		engine::renderer::begin_scene(m_game_world->camera(), non_curved_shader);
		// render the bullets
		for (auto bullet : m_game_world->get_bullets()->get_bullets())
			engine::renderer::submit(non_curved_shader, bullet->object(), glm::mat4(1.0f), true, false);

		// render the rest of the grid
		for (uint32_t i = 2; i < m_game_world->grid().size(); i++)
			engine::renderer::submit(non_curved_shader, m_game_world->grid().at(i), glm::mat4(1.0f), true, false);
	}
	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_game_world->camera(), non_curved_shader);

	for (uint32_t i = 0; i < 2; i++)
		engine::renderer::submit(non_curved_shader, m_game_world->grid().at(i), glm::mat4(1.0f), true, false);
		

	engine::renderer::end_scene();


	engine::renderer::begin_scene(m_game_world->camera(), shader);

	// render the asteroids
	for (auto asteroid : m_game_world->get_asteroids()->objects())
		engine::renderer::submit_curved_texture(shader, asteroid, m_game_world->tess_parameter(), K);

	// render the player's spaceship
	engine::renderer::submit_curved_texture(shader, m_game_world->get_spaceship()->object(), m_game_world->tess_parameter(), K);
		
	engine::renderer::end_scene();


	

	count_frames();
}

void asteroids_layer::on_event(engine::event& event) {}

void asteroids_layer::count_frames()
{
	frame_count++;
	if (frame_timer > 1.f)
	{
		frame_timer = 0.f;
		m_game_world->set_fps(frame_count);
		frame_count = 0;
	}
}
