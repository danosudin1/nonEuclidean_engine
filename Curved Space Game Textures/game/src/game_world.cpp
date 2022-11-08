#include "game_world.h"
#include "platform/opengl/gl_shader.h"

game_world::game_world() : m_2d_camera(-(float)engine::application::window().width() / 2.f, (float)engine::application::window().width() / 2.f,
	-(float)engine::application::window().height() / 2.f, (float)engine::application::window().height() / 2.f)
{
	// Hide the mouse and lock it inside the window
	//engine::application::window().hide_mouse_cursor();

	// Initialise the shaders, materials and lights
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text");
	auto non_c_t_shader = engine::renderer::shaders_library()->get("non_curved_tesselation_shader");

	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));

	std::dynamic_pointer_cast<engine::gl_shader>(non_c_t_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(non_c_t_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(non_c_t_shader)->set_uniform("transparency", 1.0f);

	m_background = engine::quad::create(glm::vec2((float)engine::application::window().width() / 2.f, (float)engine::application::window().height() / 2.f));
	m_background_black_texture = engine::texture_2d::create("assets/textures/background_black.jpg", true);
	m_background_white_texture = engine::texture_2d::create("assets/textures/background_white.jpg", true);
	simple_red_texture = engine::texture_2d::create("assets/textures/red.bmp", true);
	engine::ref<engine::texture_2d> skyline = engine::texture_2d::create("assets/textures/SkyboxLeft.bmp", true);
	//https://opengameart.org/content/asteroids
	//https://opengameart.org/content/simple-spaceship

	engine::game_object_properties square_properties;
	float square_radius = std::sqrt(100.f * 100.f + 100.f * 100.f);
	engine::polygon square_polygon = engine::polygon(square_radius, 4);
	m_asteroid_shapes.push_back(engine::shape::create(true, square_polygon.points(), square_radius));
	square_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	square_properties.position = engine::polar_vector2(200.f, -0.8f);
	square_properties.rotation = m_pi / 2.0f;
	square_properties.colour = glm::vec3(0.f);
	square_properties.texture = m_background_black_texture;
	//m_game_objects.push_back(engine::game_object::create(square_properties));

	/*engine::game_object_properties square_properties;
	float square_radius = std::sqrt(100.f * 100.f + 100.f * 100.f);
	engine::polygon square_polygon = engine::polygon(square_radius, 4);
	std::vector<glm::vec2> tex_coords_square = { glm::vec2(1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec2(1.f, 1.f) };
	m_asteroid_shapes.push_back(engine::shape::create(true, square_polygon.points(), square_radius, tex_coords_square));
	square_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	square_properties.position = engine::polar_vector2(200.f, -0.8f);
	square_properties.rotation = m_pi/2.0f;
	square_properties.colour = glm::vec3(0.f);
	square_properties.texture = m_background_black_texture;*/
	//m_game_objects.push_back(engine::game_object::create(square_properties));

	engine::game_object_properties hexagon_properties;
	float hexagon_radius = 100.f;
	engine::polygon hexagon_polygon = engine::polygon(hexagon_radius, 6);
	std::vector<glm::vec2> tex_coords_hex = { glm::vec2(1.f, 0.5f), glm::vec2(0.75f, 0.f), glm::vec2(0.25f, 0.f), glm::vec2(0.f, 0.5f), glm::vec2(0.25, 1.f), glm::vec2(0.75, 1.f) };
	m_asteroid_shapes.push_back(engine::shape::create(true, hexagon_polygon.points(), hexagon_radius, tex_coords_hex));
	hexagon_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	hexagon_properties.position = engine::polar_vector2(310.f, 0.f);
	hexagon_properties.rotation = 0.f;
	hexagon_properties.colour = glm::vec3(1.f, 0.f, 0.f);
	hexagon_properties.texture = m_background_black_texture;
	//m_game_objects.push_back(engine::game_object::create(hexagon_properties));

	square_radius = std::sqrt(100.f*100.f + 100.f*100.f);
	for (uint32_t i = 0; i < 10; i++) {
		engine::polygon square_polygon = engine::polygon(square_radius/10*(10-i), 4);
		m_asteroid_shapes.push_back(engine::shape::create(true, square_polygon.points(), square_radius / 10 * (10 - i)));
		square_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
		square_properties.position = engine::polar_vector2(310.f, 0.f);
		square_properties.rotation = 0.f;
		square_properties.colour = glm::vec3(0.f);
		square_properties.texture = m_background_white_texture;
		m_hex_and_grid.push_back(engine::game_object::create(square_properties));
	}
	
	/*engine::game_object_properties hexagon_properties;
	float hexagon_radius = 100.f;
	engine::polygon hexagon_polygon = engine::polygon(hexagon_radius, 6);
	m_asteroid_shapes.push_back(engine::shape::create(true, hexagon_polygon.points(), hexagon_radius));
	hexagon_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	hexagon_properties.position = engine::polar_vector2(310.f, 0.f);
	hexagon_properties.rotation = 0.f;
	hexagon_properties.colour = glm::vec3(1.f, 0.f, 0.f);
	m_hex_and_grid.push_back(engine::game_object::create(hexagon_properties));

	hexagon_radius = 100.f;
	for (uint32_t i = 0; i < 10; i++) {
		hexagon_polygon = engine::polygon(hexagon_radius / 10 * (10 - i), 6);
		m_asteroid_shapes.push_back(engine::shape::create(true, hexagon_polygon.points(), hexagon_radius / 10 * (10 - i)));
		hexagon_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
		hexagon_properties.position = engine::polar_vector2(310.f, 0.f);
		hexagon_properties.rotation = 0.f;
		hexagon_properties.colour = glm::vec3(0.f);
		m_concentric_hex.push_back(engine::game_object::create(hexagon_properties));
	}*/

	float arrow_max = 50.0f;
	for (uint32_t i = 0; i < 10; i++) {
		engine::polar_vector2 arrow_points[] = {
			engine::polar_vector2(arrow_max / 10 * (10 - i), 0.0f),
			engine::polar_vector2(2.0f * arrow_max / 10 * (10 - i), 0.0f),
			engine::polar_vector2(2.0f * arrow_max / 10 * (10 - i), (float)DEG2RAD * 90.0f),
			engine::polar_vector2(2.0f * arrow_max / 10 * (10 - i), (float)DEG2RAD * 180.0f),
			engine::polar_vector2(arrow_max / 10 * (10 - i), (float)DEG2RAD * 180.0f),
			engine::polar_vector2(2.5f * arrow_max / 10 * (10 - i), (float)DEG2RAD * 240.0f),
			engine::polar_vector2(2.5f * arrow_max / 10 * (10 - i), (float)DEG2RAD * 300.0f),
		};
		m_asteroid_shapes.push_back(engine::shape::create(true, engine::polar_vector2_list(arrow_points, arrow_points + sizeof(arrow_points) / sizeof(engine::polar_vector2)), arrow_max / 10 * (10 - i)));
		engine::game_object_properties arrow_properties;
		arrow_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
		arrow_properties.colour = glm::vec3(0.f);
		arrow_properties.position = engine::polar_vector2(310.f, 0.f);
		arrow_properties.rotation = 0.f;
		m_concentric_arrows.push_back(engine::game_object::create(arrow_properties));
	}
	
	//m_game_objects = m_hex_and_grid;

	/* float arrow_max = 20.0f;
	engine::polar_vector2 arrow_points[] = {
		engine::polar_vector2(arrow_max, 0.0f),
		engine::polar_vector2(2.0f * arrow_max, 0.0f),
		engine::polar_vector2(2.0f * arrow_max, (float)DEG2RAD * 90.0f),
		engine::polar_vector2(2.0f * arrow_max, (float)DEG2RAD * 180.0f),
		engine::polar_vector2(arrow_max, (float)DEG2RAD * 180.0f),
		engine::polar_vector2(2.5f * arrow_max, (float)DEG2RAD * 240.0f),
		engine::polar_vector2(2.5f * arrow_max, (float)DEG2RAD * 300.0f),
	};
	m_asteroid_shapes.push_back(engine::shape::create(true, engine::polar_vector2_list(arrow_points, arrow_points + sizeof(arrow_points) / sizeof(engine::polar_vector2)), arrow_max));
	engine::game_object_properties arrow_properties;
	arrow_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	arrow_properties.colour = glm::vec3(1.f, 0.5f, 0.0f);
	arrow_properties.position = engine::polar_vector2(280.f, 1.3f);
	arrow_properties.rotation = m_pi/4.0f;
	arrow_properties.bounding_circle = arrow_max*2.f;
	m_game_objects.push_back(engine::game_object::create(arrow_properties));

	float cross_max = 20.0f;
	engine::polar_vector2 cross_points[] = {
		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 26.6f),
		engine::polar_vector2(1.41f * cross_max, (float)DEG2RAD * 45.f),
		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 63.4f),

		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 116.6f),
		engine::polar_vector2(1.41f * cross_max, (float)DEG2RAD * 135.f),
		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 153.4f),

		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 206.6f),
		engine::polar_vector2(1.41f * cross_max, (float)DEG2RAD * 225.f),
		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 243.4f),

		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 296.6f),
		engine::polar_vector2(1.41f * cross_max, (float)DEG2RAD * 315.f),
		engine::polar_vector2(2.24f * cross_max, (float)DEG2RAD * 333.4f),
	};
	m_asteroid_shapes.push_back(engine::shape::create(true, engine::polar_vector2_list(cross_points, cross_points + sizeof(cross_points) / sizeof(engine::polar_vector2)), cross_max));
	engine::game_object_properties cross_properties;
	cross_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	cross_properties.colour = glm::vec3(1.f, 0.f, 0.f);
	cross_properties.position = engine::polar_vector2(350.f, 4.f);
	cross_properties.rotation = m_pi / 3.0f;
	cross_properties.bounding_circle = cross_max * 2.0f;
	m_game_objects.push_back(engine::game_object::create(cross_properties));

	float star_max = 25.0f;
	float star_increment = 36.f;
	engine::polar_vector2 star_points[] = {
		engine::polar_vector2(3.f * star_max, (float)DEG2RAD * star_increment * 0),
		engine::polar_vector2(1.12f * star_max, (float)DEG2RAD * star_increment * 1),

		engine::polar_vector2(3.f * star_max, (float)DEG2RAD * star_increment * 2),
		engine::polar_vector2(1.12f * star_max, (float)DEG2RAD * star_increment * 3),

		engine::polar_vector2(3.f * star_max, (float)DEG2RAD * star_increment * 4),
		engine::polar_vector2(1.12f * star_max, (float)DEG2RAD * star_increment * 5),

		engine::polar_vector2(3.f * star_max, (float)DEG2RAD * star_increment * 6),
		engine::polar_vector2(1.12f * star_max, (float)DEG2RAD * star_increment * 7),

		engine::polar_vector2(3.f * star_max, (float)DEG2RAD * star_increment * 8),
		engine::polar_vector2(1.12f * star_max, (float)DEG2RAD * star_increment * 9),
	};
	m_asteroid_shapes.push_back(engine::shape::create(true, engine::polar_vector2_list(star_points, star_points + sizeof(star_points) / sizeof(engine::polar_vector2)), star_max));
	engine::game_object_properties star_properties;
	star_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	star_properties.colour = glm::vec3(1.f, 0.f, 1.f);
	star_properties.position = engine::polar_vector2(350.f, 3.f);
	star_properties.rotation = m_pi / 3.0f;
	m_game_objects.push_back(engine::game_object::create(star_properties));

	float crescent_max = 35.0f;
	int crescent_increment = 3;
	engine::polar_vector2_list crescent_points;
	for (int i = 60 / crescent_increment; i < 300 / crescent_increment; i++) {
		crescent_points.push_back(engine::polar_vector2(crescent_max*1.5f, DEG2RAD * (crescent_increment * i)) + engine::polar_vector2(crescent_max/1.5f, 0.f));
	}
	for (int i = 330 / crescent_increment; i > 30 / crescent_increment; i--) {
		crescent_points.push_back(engine::polar_vector2(crescent_max*2.f, DEG2RAD * (crescent_increment * i)));
	}
	m_asteroid_shapes.push_back(engine::shape::create(true, crescent_points, crescent_max));
	engine::game_object_properties crescent_properties;
	crescent_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	crescent_properties.colour = glm::vec3(0.f, 1.f, 1.f);
	crescent_properties.position = engine::polar_vector2(300.f, 5.8f);
	crescent_properties.rotation = 0.f;
	crescent_properties.bounding_circle = crescent_max * 2.f;
	m_game_objects.push_back(engine::game_object::create(crescent_properties));

	float paral_max = 60.0f;
	engine::polar_vector2 paral_points[] = {
		engine::polar_vector2(paral_max, (float)DEG2RAD * 45.f),
		engine::polar_vector2(paral_max, (float)DEG2RAD * 135.f),

		engine::polar_vector2(paral_max, (float)DEG2RAD * 225.f) + engine::polar_vector2(paral_max/2.f, 0.f),
		engine::polar_vector2(paral_max, (float)DEG2RAD * 315.f) + engine::polar_vector2(paral_max/2.f, 0.f),
	};
	m_asteroid_shapes.push_back(engine::shape::create(true, engine::polar_vector2_list(paral_points, paral_points + sizeof(paral_points) / sizeof(engine::polar_vector2)), paral_max));
	engine::game_object_properties paral_properties;
	paral_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	paral_properties.colour = glm::vec3(0.5f, 0.5f, 1.f);
	paral_properties.position = engine::polar_vector2(220.f, 3.6f);
	paral_properties.rotation = 1.7;
	m_game_objects.push_back(engine::game_object::create(paral_properties));

	engine::game_object_properties asteroid_properties;
	float asteroid_radius = 70.f;
	for (uint32_t i = 0; i < 3; i++)
	{
		int j = 0;
		if (i == 2) j = 3;
		engine::polygon asteroid_polygon = engine::polygon(asteroid_radius, 5-i+j);
		m_asteroid_shapes.push_back(engine::shape::create(true, asteroid_polygon.points(), asteroid_radius));
		asteroid_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
		if(i==0)
			asteroid_properties.position = engine::polar_vector2(310.f, 0.6f);
		if(i==1)
			asteroid_properties.position = engine::polar_vector2(240.f, 2.3f);
		if(i==2)
			asteroid_properties.position = engine::polar_vector2(280.f, 4.8f);
		asteroid_properties.rotation = float(DEG2RAD * (rand() % 360));
		asteroid_properties.colour = glm::vec3(i%2*0.5f, (i+j+1)%2*0.5f, (i-j)%2*0.5f);
		m_game_objects.push_back(engine::game_object::create(asteroid_properties));
	}

	float spaceship_max = 50.0f;
	engine::polar_vector2 spaceship_points[] = {
		engine::polar_vector2(spaceship_max, 0.0f),
		engine::polar_vector2(spaceship_max, (float)DEG2RAD * 150.0f),
		engine::polar_vector2(spaceship_max / 2.0f, (float)DEG2RAD * 180.0f),
		engine::polar_vector2(spaceship_max, (float)DEG2RAD * 210.0f),
		engine::polar_vector2(spaceship_max, 0.0f)
	};
	m_spaceship_shape = engine::shape::create(false, engine::polar_vector2_list(spaceship_points, spaceship_points + sizeof(spaceship_points) / sizeof(engine::polar_vector2)), spaceship_max);
	engine::game_object_properties spaceship_properties;
	spaceship_properties.shape = m_spaceship_shape;
	spaceship_properties.colour = glm::vec3(0.0f, 1.0f, 0.0f);
	spaceship_properties.position = engine::polar_vector2(340.f, 1.70f);
	spaceship_properties.rotation = -1.f;
	m_game_objects.push_back(engine::game_object::create(spaceship_properties));


	
	engine::polar_vector2 pythagoras_points[] = {
		engine::polar_vector2::ToPolar(90.f, -30.f),
		engine::polar_vector2::ToPolar(180.f, 90.f),
		engine::polar_vector2::ToPolar(60.f, 180.f),
		engine::polar_vector2::ToPolar(-30.f, 60.f),
		engine::polar_vector2::ToPolar(-120.f, 60.f),
		engine::polar_vector2::ToPolar(-120.f, -30.f),
		engine::polar_vector2::ToPolar(-30.f, -30.f),
		engine::polar_vector2::ToPolar(-30.f, -150.f),
		engine::polar_vector2::ToPolar(90.f, -150.f),
		engine::polar_vector2::ToPolar(90.f, -30.f),
		engine::polar_vector2::ToPolar(-30.f, 60.f),
		engine::polar_vector2::ToPolar(-30.f, -30.f),
		engine::polar_vector2::ToPolar(90.f, -30.f)
	};
	m_asteroid_shapes.push_back(engine::shape::create(true, engine::polar_vector2_list(pythagoras_points, pythagoras_points + sizeof(pythagoras_points) / sizeof(engine::polar_vector2)), 70.f));
	engine::game_object_properties pythagoras_properties;
	pythagoras_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	pythagoras_properties.colour = glm::vec3(0.0f, 0.0f, 0.0f);
	pythagoras_properties.position = engine::polar_vector2(-20.f, 0.f);
	pythagoras_properties.rotation = 0.f;
	//pythagoras_properties.position = engine::polar_vector2::ToPolar(-20.f, 105.f);
	//pythagoras_properties.rotation = (90-53.13f)/180.f*m_pi-atan2f(105.f, 20.f );
	m_game_objects.push_back(engine::game_object::create(pythagoras_properties));


	/*
	engine::polar_vector2 pythagoras_line_points[] = {
		engine::polar_vector2::ToPolar(-30.f, -30.f),
		engine::polar_vector2::ToPolar(103.2f, 147.6f)
	};
	m_asteroid_shapes.push_back(engine::shape::create(false, engine::polar_vector2_list(pythagoras_line_points, pythagoras_line_points + sizeof(pythagoras_line_points) / sizeof(engine::polar_vector2)), 70.f));
	engine::game_object_properties pythagoras_line_properties;
	pythagoras_line_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	pythagoras_line_properties.colour = glm::vec3(1.0f, 0.0f, 0.0f);
	pythagoras_line_properties.position = pythagoras_properties.position;
	pythagoras_line_properties.rotation = pythagoras_properties.rotation;
	m_game_objects.push_back(engine::game_object::create(pythagoras_line_properties));

	
	
	engine::polar_vector2 pythagoras_line_points_1[] = {
		engine::polar_vector2::ToPolar(-30.f, 60.f),
		engine::polar_vector2::ToPolar(90.f, -150.f)
	};
	m_asteroid_shapes.push_back(engine::shape::create(false, engine::polar_vector2_list(pythagoras_line_points_1, pythagoras_line_points_1 + sizeof(pythagoras_line_points_1) / sizeof(engine::polar_vector2)), 70.f));
	engine::game_object_properties pythagoras_line_properties_1;
	pythagoras_line_properties_1.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	pythagoras_line_properties_1.colour = glm::vec3(1.f, 0.5f, 0.0f);
	pythagoras_line_properties_1.position = pythagoras_properties.position;
	pythagoras_line_properties_1.rotation = pythagoras_properties.rotation;
	m_game_objects.push_back(engine::game_object::create(pythagoras_line_properties_1));

	engine::polar_vector2 pythagoras_line_points_2[] = {
		engine::polar_vector2::ToPolar(-30.f, -30.f),
		engine::polar_vector2::ToPolar(180.f, 90.f)
	};
	m_asteroid_shapes.push_back(engine::shape::create(false, engine::polar_vector2_list(pythagoras_line_points_2, pythagoras_line_points_2 + sizeof(pythagoras_line_points_2) / sizeof(engine::polar_vector2)), 70.f));
	engine::game_object_properties pythagoras_line_properties_2;
	pythagoras_line_properties_2.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	pythagoras_line_properties_2.colour = glm::vec3(1.f, 0.5f, 0.0f);
	pythagoras_line_properties_2.position = pythagoras_properties.position;
	pythagoras_line_properties_2.rotation = pythagoras_properties.rotation;
	m_game_objects.push_back(engine::game_object::create(pythagoras_line_properties_2));

	engine::polar_vector2 pythagoras_line_points_3[] = {
		engine::polar_vector2::ToPolar(-30.f, -30.f),
		engine::polar_vector2::ToPolar(60.f, 180.f)
	};
	m_asteroid_shapes.push_back(engine::shape::create(false, engine::polar_vector2_list(pythagoras_line_points_3, pythagoras_line_points_3 + sizeof(pythagoras_line_points_3) / sizeof(engine::polar_vector2)), 70.f));
	engine::game_object_properties pythagoras_line_properties_3;
	pythagoras_line_properties_3.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	pythagoras_line_properties_3.colour = glm::vec3(1.f, 0.0f, 0.5f);
	pythagoras_line_properties_3.position = pythagoras_properties.position;
	pythagoras_line_properties_3.rotation = pythagoras_properties.rotation;
	m_game_objects.push_back(engine::game_object::create(pythagoras_line_properties_3));

	engine::polar_vector2 pythagoras_line_points_4[] = {
		engine::polar_vector2::ToPolar(-120.f, 60.f),
		engine::polar_vector2::ToPolar(90.f, -30.f)
	};
	m_asteroid_shapes.push_back(engine::shape::create(false, engine::polar_vector2_list(pythagoras_line_points_4, pythagoras_line_points_4 + sizeof(pythagoras_line_points_4) / sizeof(engine::polar_vector2)), 70.f));
	engine::game_object_properties pythagoras_line_properties_4;
	pythagoras_line_properties_4.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	pythagoras_line_properties_4.colour = glm::vec3(1.f, 0.5f, 0.5f);
	pythagoras_line_properties_4.position = pythagoras_properties.position;
	pythagoras_line_properties_4.rotation = pythagoras_properties.rotation;
	m_game_objects.push_back(engine::game_object::create(pythagoras_line_properties_4));
	*/


	engine::polar_vector2 grid_line_points[3] = { engine::polar_vector2((float)engine::application::window().height() / 1.43f, (float)DEG2RAD * 180.0f), engine::polar_vector2(0.f, 0.f),
		engine::polar_vector2((float)engine::application::window().height() / 1.43f, 0.f) };
	engine::ref<engine::shape> grid_line = engine::shape::create(false, engine::polar_vector2_list(grid_line_points, grid_line_points + sizeof(grid_line_points) / sizeof(engine::polar_vector2)), 0.0f);
	create_grid(grid_line);

	// Set the seed for the rand() function to be equal to current world's m_seed variable
	srand(61);

	engine::game_object_properties asteroid_properties;
	float asteroid_radius = 100.0f;
	engine::polygon asteroid_polygon = engine::polygon(asteroid_radius, 4);
	m_asteroid_shapes.push_back(engine::shape::create(true, asteroid_polygon.points(), asteroid_radius));
	asteroid_properties.shape = m_asteroid_shapes.at(m_asteroid_shapes.size() - 1);
	for (uint32_t i = 0; i < 1000; i++)
	{
		asteroid_properties.position = engine::polar_vector2(i*0.4f, 2.f*i);
		asteroid_properties.rotation = float(DEG2RAD * (rand() % 360));
		asteroid_properties.colour = glm::vec3(i%10*0.1f, (i+7)%20*0.05f, (i+3)%5*0.2f);
		asteroid_properties.texture = simple_red_texture;
		m_game_objects.push_back(engine::game_object::create(asteroid_properties));
	}

	m_K = 0.f;

	// initialise asteroids game objects
	m_bullets = bullets::create(3.f);
	m_asteroids = asteroids::create(60.f, (float)engine::application::window().width() / 3.f);
	m_asteroids->generate_asteroids(5);
	m_spaceship = spaceship::create(30.f);
}

game_world::~game_world() {}


void game_world::create_grid(engine::ref<engine::shape> grid_line_shape) {

	int dist = (int)(((float)engine::application::window().width()) / 10.f);
	engine::game_object_properties grid_line_properties;
	grid_line_properties.shape = grid_line_shape;
	grid_line_properties.colour = glm::vec3(0.5f, 0.5f, 0.5f);
	grid_line_properties.scale = 1.f;
	m_grid.push_back(engine::game_object::create(grid_line_properties));

	grid_line_properties.rotation = float(DEG2RAD * 90.0f);
	m_grid.push_back(engine::game_object::create(grid_line_properties));

	for (int i = 0; i <= 1; i++) {
		for (int j = dist; j < (float)engine::application::window().width() / 2; j += dist) {
			grid_line_properties.position = engine::polar_vector2((float)j, float(i * DEG2RAD * 90.0f));

			m_grid.push_back(engine::game_object::create(grid_line_properties));
		}
		for (int j = -dist; j > -(float)engine::application::window().width() / 2; j -= dist) {
			grid_line_properties.position = engine::polar_vector2((float)j, float(i * DEG2RAD * 90.0f));

			m_grid.push_back(engine::game_object::create(grid_line_properties));
		}
	}
}

engine::ref<game_world> game_world::create()
{
	return std::make_shared<game_world>();
}
