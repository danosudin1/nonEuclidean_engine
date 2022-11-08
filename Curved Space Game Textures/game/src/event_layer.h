#pragma once
#include <engine.h>

class game_world;

class event_layer : public engine::layer
{
public:
	event_layer(engine::ref<game_world> game_world);
	~event_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:

	engine::ref<game_world> m_game_world{};
};
