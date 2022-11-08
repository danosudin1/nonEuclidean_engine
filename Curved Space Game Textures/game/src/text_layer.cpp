#include "text_layer.h"

#include "game_world.h"

text_layer::text_layer(engine::ref<game_world> game_world, engine::ref<engine::text_manager> text_manager)
	:m_game_world(game_world), m_text_manager(text_manager) {}

text_layer::~text_layer() {}

void text_layer::on_update(const engine::timestep& time_step)
{
}

void text_layer::on_render()
{
	
	if (!m_game_world->testing_mode() && m_game_world->is_text_active())
	{
		// Render text
		const auto text_shader = engine::renderer::shaders_library()->get("text");

// Right side
		m_text_manager->render_text(text_shader, "Score: " + engine::text_manager::set_precision(m_game_world->score(), 0),
			(float)engine::application::window().width() - 30.f, (float)engine::application::window().height() - 45.f, 0.8f, glm::vec4(1.f, 1.f, 1.f, 1.f), false);

		int seconds = m_game_world->timer();
		int minutes = seconds / 60;
		seconds = seconds % 60;

		std::string timer = std::to_string(minutes) + ":";
		if (seconds < 10)
			timer = timer + "0";
		timer = timer + std::to_string(seconds);

		// Left side
		//m_text_manager->render_text(text_shader, "Time Elapsed: " + timer,
			//10.f, (float)engine::application::window().height() - 25.f * 2, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));

		std::string curvature = "No Curvature";
		if(m_game_world->get_K() == 1.f)
			curvature = "Spherical";
		if (m_game_world->get_K() == -1.f)
			curvature = "Hyperbolic";

		//m_text_manager->render_text(text_shader, "Current curvature: " + curvature,
			//10.f, (float)engine::application::window().height() - 25.f * 3, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));

		if (m_game_world->get_game_state() == game_world::game_state::start)
		{
			m_text_manager->render_text(text_shader, "Welcome to Non-Euclidean Snake",
				(float)engine::application::window().width() / 2.f - 350.f, (float)engine::application::window().height()/2.f +30.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
			m_text_manager->render_text(text_shader, "Press ENTER to start the game",
				(float)engine::application::window().width() / 2.f - 200.f, (float)engine::application::window().height() / 2.f - 30.f, 0.6f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		}
		else if (m_game_world->get_game_state() == game_world::game_state::stopped)
		{
			m_text_manager->render_text(text_shader, "Game Over",
				(float)engine::application::window().width() / 2.f - 120.f, (float)engine::application::window().height() / 2.f + 30.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
			m_text_manager->render_text(text_shader, "your score is: " + engine::text_manager::set_precision(m_game_world->score(), 0),
				(float)engine::application::window().width() / 2.f - 100.f, (float)engine::application::window().height() / 2.f - 25.f, 0.6f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
			m_text_manager->render_text(text_shader, "Press BACKSPACE to replay",
				(float)engine::application::window().width() / 2.f - 190.f, (float)engine::application::window().height() / 2.f - 60.f, 0.6f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
			m_text_manager->render_text(text_shader, "Press ENTER to return to menu",
				(float)engine::application::window().width() / 2.f - 210.f, (float)engine::application::window().height() / 2.f - 95.f, 0.6f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		}

		/**
		// Right side
		m_text_manager->render_text(text_shader, "Spaceship properties",
			(float)engine::application::window().width() - 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f), false);

		m_text_manager->render_text(text_shader, "rotation: " + engine::text_manager::set_precision(m_game_world->game_objects().at(0)->rotation(), 2),
			(float)engine::application::window().width() - 10.f, (float)engine::application::window().height() - 25.f * 2, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f), false);

		m_text_manager->render_text(text_shader, "r position: " + engine::text_manager::set_precision(m_game_world->game_objects().at(0)->position().r, 2),
			(float)engine::application::window().width() - 10.f, (float)engine::application::window().height() - 25.f * 3, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f), false);

		m_text_manager->render_text(text_shader, "theta position: " + engine::text_manager::set_precision(m_game_world->game_objects().at(0)->position().theta / m_pi * 180.f, 2),
			(float)engine::application::window().width() - 10.f, (float)engine::application::window().height() - 25.f * 4, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f), false);

		m_text_manager->render_text(text_shader, "max distance: " + engine::text_manager::set_precision(((float)engine::application::window().height() - 25.f) / 2.f, 2),
			(float)engine::application::window().width() - 10.f, (float)engine::application::window().height() - 25.f * 5, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f), false);

		// Left side
		m_text_manager->render_text(text_shader, m_game_world->layer_names().at(m_game_world->active_layer_index()),
			10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));

		m_text_manager->render_text(text_shader, "FPS: " + std::to_string(m_game_world->fps()),
			10.f, (float)engine::application::window().height() - 25.f * 2, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));

		m_text_manager->render_text(text_shader, "Current curvature: " + engine::text_manager::set_precision(m_game_world->get_K(), 2),
			10.f, (float)engine::application::window().height() - 25.f * 3, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));

		m_text_manager->render_text(text_shader, "'L' - increase K, 'K' - decrease K",
			10.f, (float)engine::application::window().height() - 25.f * 4, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));

		m_text_manager->render_text(text_shader, "'I' K = -1.0, 'O' K = 0, 'P' K = 1.0",
			10.f, (float)engine::application::window().height() - 25.f * 5, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		*/
	}
	else
	{
		// Render text
		//const auto text_shader = engine::renderer::shaders_library()->get("text");

		//m_text_manager->render_text(text_shader, "FPS: " + std::to_string(m_game_world->fps()),
		//	10.f, (float)engine::application::window().height() - 25.f * 2, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
}

void text_layer::on_event(engine::event& event)
{
}
