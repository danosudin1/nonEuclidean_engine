#pragma once
#include <engine.h>

class game_world;

class menu_layer : public engine::layer
{
public:
	menu_layer(engine::ref<game_world> game_world, engine::ref<engine::text_manager> text_manager, std::vector<engine::layer*> layers);
	~menu_layer();

	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;

private:

	engine::ref<game_world> m_game_world{};
	engine::ref<engine::text_manager>	m_text_manager{};
	std::vector<engine::layer*> m_layers;

	int option = 0;
	int game_choice = 0;
	int selection_screen = 0;

};
