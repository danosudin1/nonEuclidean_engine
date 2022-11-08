#include "bullets.h"

bullet::bullet(const engine::ref<engine::game_object>& object, float life) : m_object(object), m_life(life) {}

bullet::~bullet() {}

engine::ref<bullet> bullet::create(const engine::ref<engine::game_object>& object, float life)
{
	return std::make_shared<bullet>(object, life);
}

bullets::bullets(float size)
{
	engine::polar_vector2_list points;

	points.push_back(engine::polar_vector2(size, 0.0f));
	points.push_back(engine::polar_vector2(size, m_pi));

	engine::game_object_properties props;
	m_bullet_shape = engine::shape::create(true, points, size);
}

bullets::~bullets() {}

void bullets::on_update(const engine::timestep& time_step, int screenMaxDist, float K, bool updatePosition)
{
	if (m_cooldown > 0.f)
		m_cooldown -= (float)time_step;

	for (uint32_t i = 0; i < m_bullets.size();)
	{
		if (m_bullets.at(i)->life() > 0.f)
		{
			m_bullets.at(i)->decrement_life(time_step);
			m_bullets.at(i)->object()->on_update(time_step, screenMaxDist, K, updatePosition);
			++i;
		}
		else
		{
			m_bullets.erase(m_bullets.begin() + i);
		}
	}
}

void bullets::shoot(const engine::ref<engine::game_object>& spaceship)
{
	if (m_cooldown <= 0.f)
	{
		engine::game_object_properties bullet_props;
		bullet_props.shape = m_bullet_shape;
		bullet_props.colour = glm::vec3(0.1f, 1.f, 0.1f);
		bullet_props.position = spaceship->position();
		bullet_props.rotation = spaceship->rotation();
		bullet_props.velocity = engine::polar_vector2(m_speed, spaceship->rotation());
		m_bullets.push_back(bullet::create(engine::game_object::create(bullet_props), m_life));
		m_cooldown = cooldown_reset;
	}
}

engine::ref<bullets> bullets::create(float size)
{
	return std::make_shared<bullets>(size);
}
