#pragma once
#include <engine.h>
#include <fstream>
#include <iostream>
#include <sstream>

class game_world;

class tester_layer : public engine::layer
{
public:
	tester_layer(engine::ref<game_world> game_world, std::vector<engine::layer*> layers);
	~tester_layer();

	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;

	void next_test();
	void write_to_log_file();

	const bool& switch_layer() const { return s_switch_layer; }
	void turn_switch_layer_off() { s_switch_layer = false; }

private:

	std::ofstream myfile;

	float m_test_timer = 0.f;
	float m_timer_end = 0.1f;

	int num_cycles = 9;
	int current_cycle = 0;

	double m_fps_sum = 0.0;
	int count_sum_instances = 0;
	std::vector<double> m_fps_average_sum;
	std::vector<std::string> m_record_info;
	int record_index = 0;

	bool s_switch_layer = false;

	engine::ref<game_world> m_game_world{};

	std::vector<engine::layer*> m_layers;

	int max_tessellation = 90;
	int increment_counter = 0;
	int increment = 15;

	int num_obj = 100;
};
