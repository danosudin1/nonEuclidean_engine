#include "player.h"
#include "engine/events/key_event.h"

player::player() {}

player::~player() {}

void player::on_event(const engine::timestep& time_step)
{
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {
		m_object->set_acceleration(engine::polar_vector2(1.0f, m_object->rotation()));
	}

	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		
	}

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		m_object->set_angular_velocity(1.0f);
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		m_object->set_angular_velocity(-0.1f);
	}
	if (GetKeyState(VK_RETURN) & 0x80) {
		//playerCentered = true;
	}
}

void player::on_update(const engine::timestep& time_step) {
	m_object->set_acceleration(engine::polar_vector2(0.0f, 0.0f));
	m_object->set_angular_velocity(0.0f);
	on_event(time_step);
}

bool player::check_collision() {

	// Calculate the position and next position of player
	return false;

}
