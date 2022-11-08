#include "event_layer.h"
#include "game_world.h"
#include "engine/events/key_event.h"

event_layer::event_layer(engine::ref<game_world> game_world) : m_game_world(game_world) {}

event_layer::~event_layer() {}

void event_layer::on_update(const engine::timestep& time_step)
{
  if (m_game_world->start_timer() > 0.f)
		m_game_world->set_start_timer(m_game_world->start_timer() - (float)time_step);

	if (m_game_world->is_re_run())
	{
		m_game_world->set_score(0.f);
		m_game_world->get_asteroids()->reset();
		m_game_world->get_spaceship()->reset();
		m_game_world->set_game_state(game_world::game_state::running);
		m_game_world->switch_re_run();
	}
} 

void event_layer::on_render()
{
	engine::render_command::clear_color({ 0.3f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();
	if (!m_game_world->testing_mode())
	{
		// Set up material shader. (does not render textures, renders materials instead)
		const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
		engine::renderer::begin_scene(m_game_world->camera(), textured_lighting_shader);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(0.f, 0.f, -0.2f));

		if (m_game_world->is_background_white())
			m_game_world->white_texture()->bind();
		else
			m_game_world->black_texture()->bind();

		engine::renderer::submit(textured_lighting_shader, m_game_world->background()->mesh()->va(), transform, false, 3);

		engine::renderer::end_scene();
	}
	
} 

void event_layer::on_event(engine::event& event)
{
	if (m_game_world->is_asteroids())
	{
		if (GetKeyState(VK_SPACE) & 0x80)
			m_game_world->get_bullets()->shoot(m_game_world->get_spaceship()->object());

		if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)
			m_game_world->get_spaceship()->object()->set_acceleration(m_game_world->get_spaceship()->object()->acceleration() + engine::polar_vector2(100.f, m_game_world->get_spaceship()->object()->rotation()));
		else
			m_game_world->get_spaceship()->object()->set_acceleration(engine::polar_vector2(0.f, 0.f));

		if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)
			m_game_world->get_spaceship()->object()->set_angular_velocity(5.f);
		else if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)
			m_game_world->get_spaceship()->object()->set_angular_velocity(-5.f);
		else
			m_game_world->get_spaceship()->object()->set_angular_velocity(0.f);
	}
  else
  {
if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)
		m_game_world->game_objects().at(1)->set_acceleration(m_game_world->game_objects().at(1)->acceleration() + engine::polar_vector2(100.f, m_game_world->game_objects().at(1)->rotation()));
	else
		m_game_world->game_objects().at(1)->set_acceleration(engine::polar_vector2(0.f, 0.f));

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)
		m_game_world->game_objects().at(1)->set_angular_velocity(5.f);
	else if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)
		m_game_world->game_objects().at(1)->set_angular_velocity(-5.f);
	else
		m_game_world->game_objects().at(1)->set_angular_velocity(0.f);
}

	

	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);

		if (e.key_code() == engine::key_codes::KEY_TAB)
			engine::renderer_api::enable_wireframe();
		 
		if (e.key_code() == engine::key_codes::KEY_CAPS_LOCK)
			engine::renderer_api::disable_wireframe();
		
			

		if (e.key_code() == engine::key_codes::KEY_B)
			m_game_world->is_background_white();

		if (e.key_code() == engine::key_codes::KEY_T)
			m_game_world->switch_text_active();

		/*if (e.key_code() == engine::key_codes::KEY_W) {
			for(uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
				m_game_world->game_objects().at(i)->set_position(engine::polar_vector2(m_game_world->game_objects().at(i)->position().r + 10.f, m_game_world->game_objects().at(i)->position().theta));
		}
			
		if (e.key_code() == engine::key_codes::KEY_S) {
			for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
				m_game_world->game_objects().at(i)->set_position(engine::polar_vector2(m_game_world->game_objects().at(i)->position().r - 10.f, m_game_world->game_objects().at(i)->position().theta));
		}
			
		if (e.key_code() == engine::key_codes::KEY_A) {
			for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
				m_game_world->game_objects().at(i)->set_position(engine::polar_vector2(m_game_world->game_objects().at(i)->position().r, engine::polar_vector2::constrain_angle_rad(m_game_world->game_objects().at(i)->position().theta + m_pi / 8.f)));
		}
		if (e.key_code() == engine::key_codes::KEY_D) {
			for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
				m_game_world->game_objects().at(i)->set_position(engine::polar_vector2(m_game_world->game_objects().at(i)->position().r, engine::polar_vector2::constrain_angle_rad(m_game_world->game_objects().at(i)->position().theta - m_pi / 2.f)));
		}
		if (e.key_code() == engine::key_codes::KEY_E) {
			for (uint32_t i = 0; i < m_game_world->game_objects().size(); i++)
				m_game_world->game_objects().at(i)->set_rotation(engine::polar_vector2::constrain_angle_rad(m_game_world->game_objects().at(i)->rotation() + m_pi / 80.f));
		}*/

		if (e.key_code() == engine::key_codes::KEY_F)
			m_game_world->set_game_objects(m_game_world->hex_and_grid());
		if (e.key_code() == engine::key_codes::KEY_G)
			m_game_world->set_game_objects(m_game_world->concentric_hex());
		if (e.key_code() == engine::key_codes::KEY_H)
			m_game_world->set_game_objects(m_game_world->concentric_arrows());

		// Changing curvature
		if (e.key_code() == engine::key_codes::KEY_O)
			m_game_world->set_K(0.0f);
		if (e.key_code() == engine::key_codes::KEY_P)
			m_game_world->set_K(1.0f);
		if (e.key_code() == engine::key_codes::KEY_I)
			m_game_world->set_K(-1.0f);
		if (e.key_code() == engine::key_codes::KEY_L)
			m_game_world->set_K(m_game_world->get_K() + 0.01f);
		if (e.key_code() == engine::key_codes::KEY_K)
			m_game_world->set_K(m_game_world->get_K() - 0.01f);

		if (e.key_code() == engine::key_codes::KEY_ENTER && m_game_world->get_game_state() == game_world::game_state::start && m_game_world->start_timer() <= 0.f)
			m_game_world->set_game_state(game_world::game_state::running);

		if (e.key_code() == engine::key_codes::KEY_BACKSPACE && m_game_world->get_game_state() == game_world::game_state::stopped)
			m_game_world->switch_re_run();

		if (e.key_code() == engine::key_codes::KEY_0)
		{
			//if (m_game_world->get_game_state() == game_world::game_state::running)
				//m_game_world->set_game_state(game_world::game_state::paused);
			//else
			if (m_game_world->get_game_state() == game_world::game_state::paused)
				m_game_world->set_game_state(game_world::game_state::running);
		}
	}
}
