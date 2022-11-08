#include "asteroids.h"

asteroids::asteroids(float size, uint32_t max_dist) : m_max_dist(max_dist)
{
	std::vector<glm::vec2> tex_coords_hex = { glm::vec2(0.5f, 0.15f), glm::vec2(0.2f, 0.35f), glm::vec2(0.2f, 0.65f), glm::vec2(0.5f, 0.85f), glm::vec2(0.8, 0.65f), glm::vec2(0.8, 0.35f) };

	engine::polygon asteroid_polygon = engine::polygon(size, 6);
	m_asteroids_shape_big = engine::shape::create(true, asteroid_polygon.points(), size, tex_coords_hex);

	asteroid_polygon = engine::polygon(size / 2.f, 6);
	m_asteroids_shape_medium = engine::shape::create(true, asteroid_polygon.points(), size / 2.f, tex_coords_hex);

	asteroid_polygon = engine::polygon(size / 4.f, 6);
	m_asteroids_shape_small = engine::shape::create(true, asteroid_polygon.points(), size / 4.f, tex_coords_hex);
}

asteroids::~asteroids() {}

void asteroids::on_update(const engine::timestep& time_step, int screenMaxDist, float K, bool updatePosition)
{
	for (auto object : m_objects)
	{
		object->on_update(time_step, screenMaxDist, K, updatePosition);
	}
}

void asteroids::reset()
{
	m_objects.clear();
	generate_asteroids(5);
}

void asteroids::generate_asteroids(float num)
{
	srand(1);

	for (uint32_t i = 0; i < 3; i++)
	{
		for (uint32_t j = 10000; j < 10015; j++)
		{
			std::string path = "assets/textures/";
			if (i == 0)
				path = path + "large/";
			else if (i == 1)
				path = path + "medium/";
			else
				path = path + "small/";
			
			path = path + "a" + std::to_string(j) + ".png";

			engine::ref<engine::texture_2d> m_asteroid_texture = engine::texture_2d::create(path, true);

			m_textures.push_back(m_asteroid_texture);
		}
	}
	
	engine::game_object_properties asteroid_props;
	for (uint32_t i = 0; i < num; i++)
	{
		asteroid_props.shape = m_asteroids_shape_big;
		asteroid_props.colour = glm::vec3(0.5f);
		asteroid_props.position = engine::polar_vector2(50 + rand() % (m_max_dist - 50), DEG2RAD * (rand() % 360));
		asteroid_props.rotation = DEG2RAD * (rand() % 360);
		asteroid_props.velocity = engine::polar_vector2(25 + rand() % 50, DEG2RAD * (rand() % 360));
		asteroid_props.bounding_circle = m_asteroids_shape_big->bounding_sphere();
		asteroid_props.texture = m_textures.at(rand() % 15);
		m_objects.push_back(engine::game_object::create(asteroid_props));
	}
}

engine::ref<asteroids> asteroids::create(float size, uint32_t max_dist)
{
	return std::make_shared<asteroids>(size, max_dist);
}

void asteroids::check_collision(std::vector<engine::ref<bullet>> &bullets, int &score)
{
	// for each object in the vector, check collision with the bullets
	for (uint32_t i = 0; i < m_objects.size();)
	{
		float collision = false;
		uint32_t j = 0;

		while (!collision && j < bullets.size())
		{
			float distance = (m_objects.at(i)->position() - bullets.at(j)->object()->position()).r;
			if (distance < m_objects.at(i)->bounding_circle())
			{
				collision = true;
				bullets.erase(bullets.begin()+j);
			}
			else
				++j;
		}

		if (collision)
		{
			score++;
			if (m_objects.at(i)->bounding_circle() != m_asteroids_shape_small->bounding_sphere())
			{
				engine::game_object_properties asteroid_props;
				// create 2 smaller asteroids and add them to the list
				if (m_objects.at(i)->bounding_circle() == m_asteroids_shape_big->bounding_sphere())
				{
					asteroid_props.shape = m_asteroids_shape_medium;
					asteroid_props.texture = m_textures.at(rand() % 15 + 15);
				}
				else
				{
					asteroid_props.shape = m_asteroids_shape_small;
					asteroid_props.texture = m_textures.at(rand() % 15 + 30);
				}
				asteroid_props.colour = glm::vec3(0.5f);
				asteroid_props.position = m_objects.at(i)->position();
				asteroid_props.bounding_circle = asteroid_props.shape->bounding_sphere();

				float first_asteroid_direction = DEG2RAD * (rand() % 360);
				asteroid_props.rotation = DEG2RAD * (rand() % 360);
				asteroid_props.velocity = engine::polar_vector2(m_objects.at(i)->velocity().r + 25 + rand() % 25, first_asteroid_direction);
				m_objects.push_back(engine::game_object::create(asteroid_props));

				asteroid_props.rotation = DEG2RAD * (rand() % 360);
				asteroid_props.velocity = engine::polar_vector2(m_objects.at(i)->velocity().r + 25 + rand() % 25, first_asteroid_direction + DEG2RAD * (90 + rand() % 180));
				m_objects.push_back(engine::game_object::create(asteroid_props));

			}
			//remove the original asteroid
			m_objects.erase(m_objects.begin() + i);
		}
		else
			++i;
	}
}
