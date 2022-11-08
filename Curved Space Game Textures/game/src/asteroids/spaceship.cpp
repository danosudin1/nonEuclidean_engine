#pragma once
#include "spaceship.h"

spaceship::spaceship(float size)
{
	engine::ref<engine::texture_2d> m_spaceship_texture = engine::texture_2d::create("assets/textures/shuttle.png", true); //https://opengameart.org/content/simple-spaceship

	std::vector<glm::vec2> tex_coords_spaceship = { glm::vec2(0.5f, 1.f), glm::vec2(0.f, 0.15f), glm::vec2(0.5f, 0.f), glm::vec2(1.f, 0.15f), glm::vec2(0.5f, 1.f) };
	engine::polar_vector2 spaceship_points[] = {
		engine::polar_vector2(size, 0.0f),
		engine::polar_vector2(size, (float)DEG2RAD * 150.0f),
		engine::polar_vector2(size / 2.0f, (float)DEG2RAD * 180.0f),
		engine::polar_vector2(size, (float)DEG2RAD * 210.0f),
		engine::polar_vector2(size, 0.0f)
	};
	engine::game_object_properties props;
	props.shape = engine::shape::create(false, engine::polar_vector2_list(spaceship_points, spaceship_points + sizeof(spaceship_points) / sizeof(engine::polar_vector2)), size, tex_coords_spaceship);
	props.colour = glm::vec3(1.f, 0.1f, 0.1f);
	props.bounding_circle = size/2.f;
	props.position = engine::polar_vector2(310.f, 0.f);
	props.rotation = (float)DEG2RAD * -45.0f;
	props.texture = m_spaceship_texture;
	m_object = engine::game_object::create(props);
}

spaceship::~spaceship() {}

void spaceship::on_update(const engine::timestep& time_step, int screenMaxDist, float K, bool updatePosition)
{
	m_object->on_update(time_step, screenMaxDist, K, updatePosition);
	m_object->set_acceleration(engine::polar_vector2(std::min(m_object->acceleration().r, 50.f), m_object->acceleration().theta));
	m_object->set_velocity(engine::polar_vector2(m_object->velocity().r * 0.996f, m_object->velocity().theta));
}

engine::ref<spaceship> spaceship::create(float size)
{
	return std::make_shared<spaceship>(size);
}

bool spaceship::check_collision(const std::vector<engine::ref<engine::game_object>>& asteroids)
{
	// for each object in the vector, check collision with the bullets
	for (auto asteroid : asteroids)
	{
		float distance = (asteroid->position() - m_object->position()).r;
		if (distance < (asteroid->bounding_circle() + m_object->bounding_circle()/3.f))
			return true;
	}
	return false;
}

void spaceship::reset()
{
	m_object->set_position(engine::polar_vector2(310.f, 0.f));
	m_object->set_rotation((float)DEG2RAD * -45.0f);
	m_object->set_velocity(0.f);
	m_object->set_angular_velocity(0.f);
}
