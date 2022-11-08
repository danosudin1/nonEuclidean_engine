#include "menu_layer.h"
#include "engine/events/key_event.h"
#include "../game_world.h"

menu_layer::menu_layer(engine::ref<game_world> game_world, engine::ref<engine::text_manager> text_manager, std::vector<engine::layer*> layers)
	:m_game_world(game_world), m_text_manager(text_manager), m_layers(layers) {}

menu_layer::~menu_layer() {}

void menu_layer::on_update(const engine::timestep& time_step)
{
}

void menu_layer::on_render()
{
	// Render text
	const auto text_shader = engine::renderer::shaders_library()->get("text");

	if (selection_screen == 0)
	{
		m_text_manager->render_text(text_shader, "Choose the mode:",
			(float)engine::application::window().width() / 2.f - 200.f, (float)engine::application::window().height() / 2.f + 100.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Asteroids",
			(float)engine::application::window().width() / 2.f - 120.f, (float)engine::application::window().height() / 2.f + 40.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Testing",
			(float)engine::application::window().width() / 2.f - 120.f, (float)engine::application::window().height() / 2.f - 20.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}
	else if (selection_screen == 1)
	{
		m_text_manager->render_text(text_shader, "Choose the curvature:",
			(float)engine::application::window().width() / 2.f - 200.f, (float)engine::application::window().height() / 2.f + 100.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "No Curvature",
			(float)engine::application::window().width() / 2.f - 120.f, (float)engine::application::window().height() / 2.f + 40.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Spherical",
			(float)engine::application::window().width() / 2.f - 120.f, (float)engine::application::window().height() / 2.f - 20.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Hyperbolic",
			(float)engine::application::window().width() / 2.f - 120.f, (float)engine::application::window().height() / 2.f - 80.f, 1.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}

	m_text_manager->render_text(text_shader, "=>",
		(float)engine::application::window().width() / 2.f - 180.f, (float)engine::application::window().height() / 2.f + 40.f - 60.f * option, 1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));

}

void menu_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);

		if (e.key_code() == engine::key_codes::KEY_DOWN || e.key_code() == engine::key_codes::KEY_S)
			option++;
		if (e.key_code() == engine::key_codes::KEY_UP || e.key_code() == engine::key_codes::KEY_W)
			option--;

		int var = 2;
		if (selection_screen == 1)
				var = 3;
		option = option % var;
		if (option < 0)
			option += var;

		if (e.key_code() == engine::key_codes::KEY_ENTER)
		{
			if (selection_screen == 0)
			{
				selection_screen++;
				game_choice = option;
				option == 0;
			}
			else
			{
				if (option == 0)
					m_game_world->set_K(0.f);
				else if (option == 1)
					m_game_world->set_K(1.f);
				else if (option == 2)
					m_game_world->set_K(-1.f);

				m_layers.at(game_choice)->set_active(true);

				if (game_choice == 0)
				{
					m_game_world->switch_asteroids();
					m_game_world->switch_white_background();
				}
				else if (game_choice == 1)
				{
					m_game_world->toggle_testing_mode();
					m_game_world->set_active_layer_index(2);
					m_game_world->set_num_objects(0);
					m_game_world->set_tess_parameter(15);
				}
					

				set_active(false);

				m_game_world->set_game_state(game_world::game_state::start);
				m_game_world->set_start_timer(0.5f);

				m_game_world->switch_text_active();

				option = 0;
				game_choice = 0;
				selection_screen = 0;
			}
		}
	}
}
