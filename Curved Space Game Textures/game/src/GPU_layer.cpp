#include "GPU_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "game_world.h"

GPU_layer::GPU_layer(engine::ref<game_world> game_world)
    : m_game_world(game_world) {}

GPU_layer::~GPU_layer() {}

void GPU_layer::on_update(const engine::timestep& time_step) 
{
	frame_timer += (float)time_step;

	/*for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
	{
		m_game_world->game_objects().at(i)->on_update(time_step, (float)engine::application::window().width() / 2.f, m_game_world->get_K(), true);
		m_game_world->game_objects().at(i)->set_acceleration(engine::polar_vector2(std::min(m_game_world->game_objects().at(i)->acceleration().r, 20.f), m_game_world->game_objects().at(i)->acceleration().theta));
		m_game_world->game_objects().at(i)->set_velocity(engine::polar_vector2(m_game_world->game_objects().at(i)->velocity().r * 0.99f, m_game_world->game_objects().at(i)->velocity().theta));
	}*/
} 

void GPU_layer::on_render() 
{
	float K = m_game_world->get_K();
	if (!m_game_world->testing_mode())
	{
		engine::ref<engine::shader> non_curved_shader = engine::renderer::shaders_library()->get("non_curvature_shader");

		engine::renderer::begin_scene(m_game_world->camera(), non_curved_shader);

		for (uint32_t i = 0; i < 2; i++)
			engine::renderer::submit(non_curved_shader, m_game_world->grid().at(i), glm::mat4(1.0f), true, false);

		if ((K <= 0.01f && K >= -0.01f))
			for (uint32_t i = 2; i < m_game_world->grid().size(); i++)
				engine::renderer::submit(non_curved_shader, m_game_world->grid().at(i), glm::mat4(1.0f), true, false);

		engine::renderer::end_scene();

		// Set up curved shader. (does not render textures, renders curved lines instead)
		engine::ref<engine::shader> curvature_shader;
		engine::ref<engine::shader> curvature_shader_lines;
		if (K > 0.01f)
		{
			curvature_shader = engine::renderer::shaders_library()->get("curvature_shader_spherical");
			curvature_shader_lines = engine::renderer::shaders_library()->get("curvature_shader_spherical_lines");
		}
		else if (K < -0.01f)
		{
			curvature_shader = engine::renderer::shaders_library()->get("curvature_shader_hyperbolic");
			curvature_shader_lines = engine::renderer::shaders_library()->get("curvature_shader_hyperbolic_lines");
		}
		else {
			curvature_shader = engine::renderer::shaders_library()->get("non_curved_tesselation_shader");
		}


		if (!(K <= 0.01f && K >= -0.01f))
		{
			engine::renderer::begin_scene(m_game_world->camera(), curvature_shader_lines);

			for (uint32_t i = 2; i < m_game_world->grid().size(); i++)
				engine::renderer::submit_curved_lines(curvature_shader_lines, m_game_world->grid().at(i), m_game_world->tess_parameter(), K);

			engine::renderer::end_scene();
		}



		engine::renderer::begin_scene(m_game_world->camera(), curvature_shader);

		for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
		{
			if (K <= 0.01f && K >= -0.01f)
			{
				/*curvature_shader = engine::renderer::shaders_library()->get("non_curved_tesselation_shader");
				engine::renderer::begin_scene(m_game_world->camera(), curvature_shader);

				glm::mat4 transform(1.0f);

				m_game_world->game_objects().at(i)->texture()->bind();

				engine::renderer::submit(curvature_shader, m_game_world->game_objects().at(i)->shape()->mesh(), transform, false, true);

				engine::renderer::end_scene();*/

				engine::renderer::submit_non_curved_tess(curvature_shader, m_game_world->game_objects().at(0), m_game_world->tess_parameter());
			}
			//
			else
			{
				if (i == 1)
				{
					float x = 0;
				}

				engine::renderer::submit_curved_texture(curvature_shader, m_game_world->game_objects().at(0), m_game_world->tess_parameter(), K);
			}

		}
		
	}

	else
	{
		engine::ref<engine::shader> texture_shader;
		if (K > 0.01f)
			texture_shader = engine::renderer::shaders_library()->get("curvature_shader_spherical");
		else if (K < -0.01f)
			texture_shader = engine::renderer::shaders_library()->get("curvature_shader_hyperbolic");
		else
			texture_shader = engine::renderer::shaders_library()->get("non_curved_tesselation_shader");

		engine::renderer::begin_scene(m_game_world->camera(), texture_shader);

		for (uint32_t i = 0; i < m_game_world->num_objects(); i++)
		{
			if (K <= 0.01f && K >= -0.01f)
				engine::renderer::submit_non_curved_tess(texture_shader, m_game_world->game_objects().at(i), m_game_world->tess_parameter());
			else
				engine::renderer::submit_curved_texture(texture_shader, m_game_world->game_objects().at(i), m_game_world->tess_parameter(), K);
		}
	}

	engine::renderer::end_scene();

	count_frames();
} 

void GPU_layer::on_event(engine::event& event) {}

void GPU_layer::count_frames()
{
	frame_count++;
	if (frame_timer > 1.f)
	{
		frame_timer = 0.f;
		m_game_world->set_fps(frame_count);
		frame_count = 0;
	}
}
