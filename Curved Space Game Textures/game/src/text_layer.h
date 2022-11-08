#pragma once
#include <engine.h>

class game_world;

class text_layer : public engine::layer
{
public:
	text_layer(engine::ref<game_world> game_world, engine::ref<engine::text_manager> text_manager);
	~text_layer();

	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;

private:

	engine::ref<game_world> m_game_world{};
	engine::ref<engine::text_manager>	m_text_manager{};

};
