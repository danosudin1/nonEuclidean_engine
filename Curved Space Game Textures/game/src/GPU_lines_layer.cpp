#include "GPU_lines_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "game_world.h"

GPU_lines_layer::GPU_lines_layer(engine::ref<game_world> game_world)
    : m_game_world(game_world) {}

GPU_lines_layer::~GPU_lines_layer() {}

void GPU_lines_layer::on_update(const engine::timestep& time_step)
{
	frame_timer += (float)time_step;

	/*for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
	{
		m_game_world->game_objects().at(i)->on_update(time_step, (float)engine::application::window().width() / 2.f, m_game_world->get_K(), true);
	}*/
} 

void GPU_lines_layer::on_render()
{
	float K = m_game_world->get_K();

	// Set up curved shader. (does not render textures, renders curved lines instead)
	engine::ref<engine::shader> lines_shader;
	if (K > 0.01f)
		lines_shader = engine::renderer::shaders_library()->get("curvature_shader_spherical_lines");
	else if (K < -0.01f)
		lines_shader = engine::renderer::shaders_library()->get("curvature_shader_hyperbolic_lines");
	else
		lines_shader = engine::renderer::shaders_library()->get("non_curvature_shader");

	engine::renderer::begin_scene(m_game_world->camera(), lines_shader);

	if (!m_game_world->testing_mode())
	{
		for (uint32_t i = 0; i < m_game_world->grid().size(); i++)
		{
			if ((K <= 0.01f && K >= -0.01f) || i < 2)
				engine::renderer::submit(lines_shader, m_game_world->grid().at(i));
			else
				engine::renderer::submit_curved_lines(lines_shader, m_game_world->grid().at(i), m_game_world->tess_parameter(), K);
		}

		for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
		{
			if (K <= 0.01f && K >= -0.01f)
				engine::renderer::submit(lines_shader, m_game_world->game_objects().at(i));
			else
				engine::renderer::submit_curved_texture(lines_shader, m_game_world->game_objects().at(i), m_game_world->tess_parameter(), K);
		}

	}
	else
	{
		for (uint32_t i = 0; i < m_game_world->num_objects(); i++)
		{
			if (K <= 0.01f && K >= -0.01f)
				engine::renderer::submit(lines_shader, m_game_world->game_objects().at(i), glm::mat4(1.0f), true, false);
			else
				engine::renderer::submit_curved_lines(lines_shader, m_game_world->game_objects().at(i), m_game_world->tess_parameter(), K);
		}
	}
	
	engine::renderer::end_scene();

	count_frames();
} 

void GPU_lines_layer::on_event(engine::event& event)
{
}

void GPU_lines_layer::count_frames()
{
	frame_count++;
	if (frame_timer > 1.f)
	{
		frame_timer = 0.f;
		m_game_world->set_fps(frame_count);
		frame_count = 0;
	}
}
