#pragma once
#include <engine.h>
#include "asteroids/asteroids.h"
#include "asteroids/bullets.h"
#include "asteroids/spaceship.h"

class game_world
{
public:

	enum game_state
	{
		start = 0,
		running,
		stopped,
		paused
	};

	game_world();
	~game_world();

	static engine::ref<game_world> create();

	// common getters and setters
	const std::vector<engine::ref<engine::game_object>>& grid() const { return m_grid; }
	const std::vector<engine::ref<engine::game_object>>& game_objects() const { return m_game_objects; }
	const std::vector<engine::ref<engine::game_object>>& hex_and_grid() const { return m_hex_and_grid; }
	const std::vector<engine::ref<engine::game_object>>& concentric_hex() const { return m_concentric_hex; }
	const std::vector<engine::ref<engine::game_object>>& concentric_arrows() const { return m_concentric_arrows; }
	void set_game_objects(const std::vector<engine::ref<engine::game_object>>& game_objects) { m_game_objects = game_objects; }

	const engine::ref<engine::quad>& background() const { return m_background; }
	const engine::ref<engine::texture_2d>& white_texture() const { return m_background_white_texture; }
	const engine::ref<engine::texture_2d>& black_texture() const { return m_background_black_texture; }

	const std::vector<engine::ref<engine::shape>> &asteroid_shapes() const { return m_asteroid_shapes; }
	const engine::ref<engine::shape> &spaceship_shape() const { return m_spaceship_shape; }
	const engine::ref<engine::shape>& line_shape() const { return m_line_shape; }

	const engine::orthographic_camera& camera() const { return m_2d_camera; }

	const bool& is_text_active() const { return s_text_gui; }
	const bool& is_background_white() const { return s_white_background; }
	void switch_white_background() { s_white_background = !s_white_background; }
	void switch_text_active() { s_text_gui = !s_text_gui; }

	const float& get_K() const { return m_K; }
	void set_K(float K) { m_K = K; }

	const int& fps() const { return m_fps; }
	void set_fps(int fps) { m_fps = fps; }

	const int num_objects() const { return m_num_objects; }
	void set_num_objects(int num_objects) { m_num_objects = num_objects; }
	const int tess_parameter() const { return m_tessellation_parameter; }
	void set_tess_parameter(int tess_parameter) { m_tessellation_parameter = tess_parameter; }

	const bool& testing_mode() const { return s_testing_mode; }
	void toggle_testing_mode() { s_testing_mode = !s_testing_mode; }

	void set_active_layer_index(int index) { m_active_layer_index = index; }
	const int& active_layer_index() const { return m_active_layer_index; }

	const std::vector<std::string>& layer_names() const { return m_layer_names; }

	const bool& great_circle_navigation() const { return s_great_circle_navigation; }
	void set_great_circle_navigation(bool set_to) { s_great_circle_navigation = set_to; }

	const bool& non_curved_tesselation() const { return s_non_curved_tesselation; }
	void set_non_curved_tesselation(bool set_to) { s_non_curved_tesselation = set_to; }

	int& score() { return m_score; }
	void set_score(int score) { m_score = score; }

	const game_state& get_game_state() const { return m_game_state; }
	void set_game_state(game_state state) { m_game_state = state; }
	const bool& is_re_run() const { return s_re_run; }
	void switch_re_run() { s_re_run = !s_re_run; }

	const float& start_timer() const { return m_start_timer; }
	void set_start_timer(float timer) { m_start_timer = timer; }

	const float& timer() const { return m_timer; }
	void update_timer(float time) { m_timer += time; }
	void reset_timer() { m_timer = 0.f; }

	// asteroids getters
	const engine::ref<asteroids>& get_asteroids() const { return m_asteroids; }
	const engine::ref<spaceship>& get_spaceship() const { return m_spaceship; }
	const engine::ref<bullets>& get_bullets() const { return m_bullets; }

	const bool& is_snake() const { return s_snake_active; }
	void switch_snake() { s_snake_active = !s_snake_active; }

	const bool& is_asteroids() const { return s_asteroids_active; }
	void switch_asteroids() { s_asteroids_active = !s_asteroids_active; }

private:

	// common variables 
	engine::ref<engine::quad> m_background;
	engine::ref<engine::texture_2d> m_background_white_texture;
	engine::ref<engine::texture_2d> m_background_black_texture;
	engine::ref<engine::texture_2d> simple_red_texture;

	bool s_white_background = true;
	bool s_text_gui = false;

	engine::ref<engine::shape> m_spaceship_shape;
	engine::ref<engine::shape> m_line_shape;

	std::vector<engine::ref<engine::shape>> m_asteroid_shapes;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};
	std::vector<engine::ref<engine::game_object>>     m_hex_and_grid{};
	std::vector<engine::ref<engine::game_object>>     m_concentric_hex{};
	std::vector<engine::ref<engine::game_object>>     m_concentric_arrows{};

	std::vector<engine::ref<engine::game_object>>     m_grid{};

	void create_grid(engine::ref<engine::shape> grid_line_shape);

	float m_K;

	engine::orthographic_camera       m_2d_camera;

	int m_fps = 0;

	int m_num_objects = 0;
	int m_tessellation_parameter = 1;

	bool s_testing_mode = false;

	std::vector<std::string> m_layer_names = { "asteroids", "GPU_lines_layer", "GPU_textures_layer"};
	int m_active_layer_index = 0;

	bool s_great_circle_navigation = false;
	bool s_non_curved_tesselation = true;

	int m_score = 0;

	game_state m_game_state = start;

	bool s_re_run = false;

	float m_start_timer = 0.f;

	float m_timer = 0.f;

	// asteroids game objects
	engine::ref<spaceship> m_spaceship;
	engine::ref<asteroids> m_asteroids;
	engine::ref<bullets> m_bullets;

	bool s_snake_active = false;
	bool s_asteroids_active = false;
};
