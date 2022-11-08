// --------- Entry Point ---------------
#include "event_layer.h"
#include "text_layer.h"
#include "tester_layer.h"
#include "asteroids/asteroids_layer.h"
#include "asteroids/menu_layer.h"
#include "GPU_layer.h"
#include "GPU_lines_layer.h"
#include "engine/core/entry_point.h"
#include "engine/events/key_event.h"
#include "game_world.h"


class sandbox_app : public engine::application 
{
private:
	engine::ref<game_world> m_game_world;
	engine::ref<engine::text_manager> m_text_manager;
	std::vector<engine::layer*> m_layers;
	text_layer* m_text_layer;
	tester_layer* m_tester_layer;

public:
    sandbox_app() 
    {
		m_game_world = game_world::create();
		//m_game_world->toggle_testing_mode();
		m_text_manager = engine::text_manager::create();

		event_layer* m_event_layer = new event_layer(m_game_world);
		m_event_layer->set_active(true);
		push_layer(m_event_layer);

		m_layers.push_back(new asteroids_layer(m_game_world));

		m_layers.push_back(new GPU_layer(m_game_world));
		m_layers.push_back(new GPU_lines_layer(m_game_world));

		m_layers.push_back(new menu_layer(m_game_world, m_text_manager, m_layers));

		m_layers.at(3)->set_active(true);
		for (auto layer : m_layers)
		{
			push_layer(layer);
		}

		m_text_layer = new text_layer(m_game_world, m_text_manager);
		m_text_layer->set_active(true);
		push_layer(m_text_layer);

		m_tester_layer = new tester_layer(m_game_world, m_layers);
		m_tester_layer->set_active(true);
		push_layer(m_tester_layer);
    } 

	~sandbox_app() = default;

    void on_event(engine::event& event) override
    { 
        application::on_event(event); 

        engine::event_dispatcher dispatcher(event); 
        // dispatch event on window X pressed 
        dispatcher.dispatch<engine::key_pressed_event>(BIND_EVENT_FN(sandbox_app::on_key_pressed)); 
    } 

    bool on_key_pressed(engine::key_pressed_event& event) 
    { 
        if (event.event_type() == engine::event_type_e::key_pressed) 
        { 
            if (event.key_code() == engine::key_codes::KEY_ESCAPE) 
                application::exit();


			if (event.key_code() == engine::key_codes::KEY_ENTER && m_game_world->get_game_state() == game_world::game_state::stopped && !m_layers.at(3)->is_active())
			{
				if (m_layers.at(0)->is_active())
				{
					m_game_world->set_score(0.f);
					m_game_world->get_asteroids()->reset();
					m_game_world->get_spaceship()->reset();
					m_layers.at(0)->set_active(false);
					m_game_world->switch_asteroids();
					m_game_world->switch_white_background();
				}
				m_game_world->reset_timer();
				m_layers.at(3)->set_active(true);
				m_game_world->switch_text_active();
			}
/*
			if (event.key_code() == engine::key_codes::KEY_SPACE)
			{
				int active_index = -1;
				int size = 0;
				for (auto* layer : m_layers)
				{
					size++;
					if (layer->is_active())
						active_index = size-1;
				}

				m_layers.at(active_index)->set_active(false);
				if (size - 1 == active_index)
				{
					m_layers.at(0)->set_active(true);
					m_game_world->set_active_layer_index(0);
				}
				else
				{
					m_layers.at(active_index + 1)->set_active(true);
					m_game_world->set_active_layer_index(active_index + 1);
				}
			}
			if (event.key_code() == engine::key_codes::KEY_R)
			{
				if(!m_game_world->testing_mode())
					m_game_world->toggle_testing_mode();
			}
      */
            //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
        } 
        return false; 
    } 
}; 


engine::application* engine::create_application() 
{ 
    return new sandbox_app(); 
}
