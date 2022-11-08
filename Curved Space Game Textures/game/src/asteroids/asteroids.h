#pragma once
#include <engine.h>
#include "bullets.h"

class asteroids
{
public:
	asteroids(float size, uint32_t max_dist);
	~asteroids();

	const std::vector<engine::ref<engine::game_object>>& objects() const { return m_objects; }

	static engine::ref<asteroids> create(float size, uint32_t max_dist);

	void on_update(const engine::timestep& time_step, int screenMaxDist, float K, bool updatePosition);

	void reset();

	void generate_asteroids(float num);

	void check_collision(std::vector<engine::ref<bullet>> &bullets, int& score);

private:

	engine::ref<engine::shape> m_asteroids_shape_big;
	engine::ref<engine::shape> m_asteroids_shape_medium;
	engine::ref<engine::shape> m_asteroids_shape_small;

	std::vector<engine::ref<engine::game_object>> m_objects;
	std::vector<engine::ref<engine::texture_2d>> m_textures;

	uint32_t m_max_dist;
	
};
