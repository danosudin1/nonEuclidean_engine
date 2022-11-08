#pragma once
#include <engine.h>

class spaceship
{
public:
	spaceship(float size);
	~spaceship();

	const engine::ref<engine::game_object>& object() const { return m_object; }

	static engine::ref<spaceship> create(float size);

	void on_update(const engine::timestep& time_step, int screenMaxDist, float K, bool updatePosition);

	bool check_collision(const std::vector<engine::ref<engine::game_object>>& asteroids);

	void reset();

private:

	engine::ref<engine::game_object> m_object;

};
