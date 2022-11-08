#pragma once
#include <engine.h>

class game_world;

class GPU_lines_layer : public engine::layer
{
public:
	GPU_lines_layer(engine::ref<game_world> game_world);
	~GPU_lines_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:

	engine::ref<game_world> m_game_world{};

	void count_frames();
	float frame_timer = 0.0f;
	int frame_count = 0;
};
