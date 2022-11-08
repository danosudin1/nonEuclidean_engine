#pragma once

#include <engine.h>

class player {
public:
	player();										// Constructor 
	~player();										// Destructor

	void on_update(const engine::timestep& time_step);
	bool check_collision();

	// Respond to keyboard presses on arrow keys to translate the camera
	void on_event(const engine::timestep& time_step);

	void SetPlayerObject(engine::ref<engine::game_object> object) { m_object = object; }
	engine::ref<engine::game_object> GetPlayerObject() { return m_object; }


private:

	engine::ref<engine::game_object> m_object;			// The object of the player
	int m_hp;

	engine::polar_vector2 velocity;
	engine::polar_vector2 Acceleration;
};
