#pragma once
#include <engine.h>

class bullet 
{
public:
	bullet(const engine::ref<engine::game_object>& object, float life);
	~bullet();

	const engine::ref<engine::game_object>& object() const { return m_object; }
	const float& life() const { return m_life; }

	void decrement_life(const engine::timestep& time_step) { m_life -= (float)time_step; }

	static engine::ref<bullet> create(const engine::ref<engine::game_object>& object, float life);

private:
	engine::ref<engine::game_object> m_object;
	float m_life;
};

class bullets
{
public:
	bullets(float size);
	~bullets();

	std::vector<engine::ref<bullet>>& get_bullets() { return m_bullets; }

	void shoot(const engine::ref<engine::game_object>& spaceship);

	static engine::ref<bullets> create(float size);

	void on_update(const engine::timestep& time_step, int screenMaxDist, float K, bool updatePosition);

private:

	engine::ref<engine::shape> m_bullet_shape;

	std::vector<engine::ref<bullet>> m_bullets;

	float m_cooldown = 0.f;
	float cooldown_reset = 0.25f;
	float m_life = 1.5f;
	float m_speed = 400.f;
};
