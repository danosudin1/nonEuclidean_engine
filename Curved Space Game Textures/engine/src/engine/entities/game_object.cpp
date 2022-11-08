#include "pch.h"
#include "game_object.h"
#include "engine/renderer/texture.h"

// TODO - see header ;)
engine::game_object::game_object(const game_object_properties& props)
    : m_position(props.position),
    m_velocity(props.velocity),
    m_acceleration(props.acceleration),
    m_rotation(props.rotation),
	m_angular_velocity(props.angular_velocity),
	m_torque(props.torque),
    m_scale(props.scale),
    m_type(props.type),
	m_initial_angle((float)m_pi - m_position.theta),
	m_adjusted_angle((float)m_pi - m_position.theta),
	m_shape(props.shape),
	m_texture(props.texture),
	m_colour(props.colour),
	m_bounding_circle(props.bounding_circle)
{
}

engine::game_object::~game_object() {}

const void engine::game_object::transform(glm::mat4 &object_transform)
{
	// apply object's transformations 
	object_transform = glm::translate(object_transform, position().ToCartesian());
	float angle = rotation() - adjusted_angle();
	object_transform = glm::rotate(object_transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	object_transform = glm::scale(object_transform, glm::vec3(scale()));
}

void engine::game_object::bind_texture()
{
        m_texture->bind();
}

void engine::game_object::turn_towards(polar_vector2 point)
{
	m_rotation = (point - m_position).angle();
}

engine::ref<engine::game_object> engine::game_object::create(const engine::game_object_properties& props)
{
    return std::make_shared<engine::game_object>(props);
}

/** Reset this game object back to the center of the game world. */
void engine::game_object::reset()
{
	m_position = polar_vector2(0.0f, 0.0f);
	m_velocity = polar_vector2(0.0f, 0.0f);
	m_acceleration = polar_vector2(0.0f, 0.0f);
	m_rotation = 0.f;
	m_angular_velocity = 0.f;
	m_torque = 0.f;
	m_scale = 1.f;
}

/** Update this game object by updating position, velocity and angle of object. */
void engine::game_object::on_update(const timestep& time_step, int screenMaxDist, float K, bool updatePosition)
{
	if (m_acceleration.r < 0.f)
	{
		m_acceleration.r *= -1.0f;
		m_acceleration.theta += float(m_pi);
	}
	m_acceleration.theta = engine::polar_vector2::constrain_angle_rad(m_acceleration.theta);

	m_adjusted_angle = (float)m_pi - m_position.theta;
	// Calculate seconds since last update
	float t = (float)time_step;
	// Update angle
	m_rotation += m_angular_velocity * (float)t;
	m_rotation = engine::polar_vector2::constrain_angle_rad(m_rotation);

	// Update velocity
	m_velocity += m_acceleration * (float)t * 25.0f;

	if (m_velocity.r < 0.f)
	{
		m_velocity.r *= -1.0f;
		m_velocity.theta += float(m_pi);
	}
	m_velocity.theta = engine::polar_vector2::constrain_angle_rad(m_velocity.theta);

	// Update position continuously
	if (m_velocity.r > 0.0f) {
		if (K <= 0.01f && K >= -0.01f) {
			polar_vector2 mAdjustedVelocity = polar_vector2(m_velocity.r, m_velocity.theta - m_adjusted_angle);
			polar_vector2 mPreviousPosition = m_position;
			m_position += mAdjustedVelocity * (float)t;
			float additionalRotation = m_position.theta - mPreviousPosition.theta;
			m_rotation -= additionalRotation;
			m_velocity.theta -= additionalRotation;
			m_acceleration.theta -= additionalRotation;
			m_rotation = engine::polar_vector2::constrain_angle_rad(m_rotation);
		}
		else {
			float k;
			if (K > 0.01)
				k = 160.0f / sqrt(K);
			else if (K < -0.01)
				k = 160.0f / sqrt(-1.0f * K);

			float distance = m_velocity.r * (float)t;
			if (distance != 0.0f && updatePosition) {
				if (K > 0.01) m_position = spherical_position(k, distance);
				else m_position = hyperbolic_position(k, distance);
			}
		}
	}

	// if the object moves off screen, reset it to the antipodal point of the boundary
	if (m_position.r > float(screenMaxDist)) {
		m_position.r *= -1.0f;

		m_velocity.theta = engine::polar_vector2::constrain_angle_rad(m_velocity.theta + float(m_pi));
		m_acceleration.theta = engine::polar_vector2::constrain_angle_rad(m_acceleration.theta + float(m_pi));
		m_rotation = engine::polar_vector2::constrain_angle_rad(m_rotation + float(m_pi));

	}

	if (m_position.r < 0.f) {
		m_position.r *= -1.0f;
		m_position.theta += float(m_pi);
	}
	m_position.theta = engine::polar_vector2::constrain_angle_rad(m_position.theta);


	m_position.theta = engine::polar_vector2::constrain_angle_rad(m_position.theta);

	// reset the forces
	//m_acceleration = polar_vector2(0.f, 0.f);
	//m_angular_velocity = 0.f;
}

void engine::game_object::on_update_step(int screenMaxDist, float K, bool updatePosition)
{
	m_adjusted_angle = (float)m_pi - m_position.theta;

	// Update position in steps of the object velocity
	if (m_velocity.r > 0.0f) {
		if (K <= 0.01f && K >= -0.01f) {
			polar_vector2 mAdjustedVelocity = polar_vector2(m_velocity.r, m_velocity.theta - m_adjusted_angle);
			polar_vector2 mPreviousPosition = m_position;
			m_position += mAdjustedVelocity;
			float additionalRotation = m_position.theta - mPreviousPosition.theta;
			m_rotation -= additionalRotation;
			m_adjusted_angle -= additionalRotation;
			m_velocity.theta -= additionalRotation;
			m_rotation = engine::polar_vector2::constrain_angle_rad(m_rotation);
		}
		else {
			float k;
			if (K > 0.01)
				k = 160.0f / sqrt(K);
			else if (K < -0.01)
				k = 160.0f / sqrt(-1.0f * K);

			if (m_velocity.r != 0.0f && updatePosition) {
				if (K > 0.01) m_position = spherical_position(k, m_velocity.r);
				else m_position = hyperbolic_position(k, m_velocity.r);
			}
		}
	}

	// if the object moves off screen, reset it to the antipodal point of the boundary
	if (m_position.r > float(screenMaxDist)) {
		m_position.r *= -1.0f;

		m_velocity.theta = engine::polar_vector2::constrain_angle_rad(m_velocity.theta + float(m_pi));
		m_rotation = engine::polar_vector2::constrain_angle_rad(m_rotation + float(m_pi));
		if(K <= 0.01f && K >= -0.01f)
			m_adjusted_angle = engine::polar_vector2::constrain_angle_rad(m_adjusted_angle + float(m_pi));
	}
	if (m_position.r < -float(screenMaxDist)) {
		m_position.r *= -1.0f;
		m_position.theta += float(m_pi);
		m_velocity.theta = engine::polar_vector2::constrain_angle_rad(m_velocity.theta);
	}

	m_position.theta = engine::polar_vector2::constrain_angle_rad(m_position.theta);
}

engine::polar_vector2 engine::game_object::spherical_position(float scale, float distance) {

	double r_t0 = m_position.r / (double)scale;
	double cos_r_t0 = cos(r_t0);
	double sin_r_t0 = sin(r_t0);

	int direction = 1;
	double gamma_t0 = -m_velocity.theta;
	gamma_t0 = engine::polar_vector2::constrain_angle_rad((float)gamma_t0);
	if (gamma_t0 > m_pi) { gamma_t0 = 2 * m_pi - gamma_t0; direction *= -1; }
	double cos_gamma_t0 = cos(gamma_t0);

	double r_p = (double)distance / (double)scale;
	double cos_r_p = cos(r_p);
	double sin_r_p = sin(r_p);

	//using Spherical Law of Cosines
	double cos_r_t1 = cos_r_t0 * cos_r_p + sin_r_t0 * sin_r_p * cos_gamma_t0;
	if (cos_r_t1 > 1.0f) cos_r_t1 = 1.0f;
	if (cos_r_t1 < -1.0f) cos_r_t1 = -1.0f;
	double r_t1 = acos(cos_r_t1);
	double sin_r_t1 = sin(r_t1);

	double cos_delta_theta = (cos_r_p - (cos_r_t0 * cos_r_t1)) / (sin_r_t0 * sin_r_t1);
	if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
	if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;
	double delta_theta = acos(cos_delta_theta);
	double theta_t1 = m_position.theta + direction * delta_theta;

	double cos_gamma_t1_sup = (cos_r_t0 - (cos_r_p * cos_r_t1)) / (sin_r_p * sin_r_t1);
	if (cos_gamma_t1_sup > 1.0f) cos_gamma_t1_sup = 1.0f;
	if (cos_gamma_t1_sup < -1.0f) cos_gamma_t1_sup = -1.0f;
	double gamma_t1_sup = acos(cos_gamma_t1_sup);
	double gamma_t1 = m_pi - gamma_t1_sup;

	gamma_t0 = 2 * m_pi - direction * gamma_t0;
	gamma_t0 = engine::polar_vector2::constrain_angle_rad((float)gamma_t0);

	gamma_t1 = 2 * m_pi - direction * gamma_t1;
	gamma_t1 = engine::polar_vector2::constrain_angle_rad((float)gamma_t1);

	double alpha = m_rotation - gamma_t0;
	m_rotation = float(gamma_t1 + alpha);

	double gamma_diff = (float)gamma_t1 - m_velocity.theta;
	m_velocity.theta = (float)gamma_t1;
	m_acceleration.theta += gamma_diff;

	return engine::polar_vector2((float)r_t1 * scale, (float)theta_t1);
}

engine::polar_vector2 engine::game_object::hyperbolic_position(float scale, float distance) {

	if (m_position.theta < 0.7f) {
		double a = 0;
	}

	double r_t0 = m_position.r / (double)scale;
	double cosh_r_t0 = cosh(r_t0);
	double sinh_r_t0 = sinh(r_t0);

	int direction = -1;
	double gamma_t0 = m_velocity.theta;
	gamma_t0 = engine::polar_vector2::constrain_angle_rad((float)gamma_t0);
	if (gamma_t0 > m_pi) { gamma_t0 = 2 * m_pi - gamma_t0; direction *= -1; }
	double cos_gamma_t0 = cos(gamma_t0);

	double r_p = (double)distance / (double)scale;
	double cosh_r_p = cosh(r_p);
	double sinh_r_p = sinh(r_p);

	//using Hyperbolic Law of Cosines
	double cosh_r_t1 = cosh_r_t0 * cosh_r_p - sinh_r_t0 * sinh_r_p * cos_gamma_t0;
	double r_t1 = acosh(cosh_r_t1);
	double sinh_r_t1 = sinh(r_t1);

	double cos_delta_theta = ((cosh_r_t0 * cosh_r_t1) - cosh_r_p) / (sinh_r_t0 * sinh_r_t1);
	if (cos_delta_theta > 1.0f) cos_delta_theta = 1.0f;
	if (cos_delta_theta < -1.0f) cos_delta_theta = -1.0f;
	double delta_theta = acos(cos_delta_theta);
	double theta_t1 = m_position.theta + direction * delta_theta;

	double cos_gamma_t1_sup = ((cosh_r_p * cosh_r_t1) - cosh_r_t0) / (sinh_r_p * sinh_r_t1);
	if (cos_gamma_t1_sup > 1.0f) cos_gamma_t1_sup = 1.0f;
	if (cos_gamma_t1_sup < -1.0f) cos_gamma_t1_sup = -1.0f;
	double gamma_t1_sup = acos(cos_gamma_t1_sup);
	double gamma_t1 = m_pi - gamma_t1_sup;

	gamma_t1 = 2 * m_pi - direction * gamma_t1;
	gamma_t1 = engine::polar_vector2::constrain_angle_rad((float)gamma_t1);

	gamma_t0 = 2 * m_pi - direction * gamma_t0;
	gamma_t0 = engine::polar_vector2::constrain_angle_rad((float)gamma_t0);

	double alpha = m_rotation - gamma_t0;
	m_rotation = float(gamma_t1 + alpha);

	double gamma_diff = (float)gamma_t1 - m_velocity.theta;
	m_velocity.theta = (float)gamma_t1;
	m_acceleration.theta += gamma_diff;

	return engine::polar_vector2((float)r_t1 * scale, (float)theta_t1);
}

bool engine::game_object::check_collision(const engine::ref<engine::game_object> other_object, float K)
{
	float other_bounding_circle = other_object->bounding_circle();

	float distance = game_object::distance(m_position, other_object->position(), K);

	float bounding_circle_sum = other_bounding_circle + m_bounding_circle;

	return distance < bounding_circle_sum*0.99f;
}


float engine::game_object::distance(const engine::polar_vector2& position, const engine::polar_vector2& other_position, float K)
{
	
	float cos_delta_theta = cos(abs(position.theta - other_position.theta));
	
	// first, find the distance between the two objects, given their respective positions via the rule of cosines
	if (K == 0)
	{
		float dist_sq = position.r * position.r + other_position.r * other_position.r
			- 2.f * position.r * other_position.r * cos_delta_theta;
		return sqrt(dist_sq);
	}
	else
	{
		float k;
		if (K > 0.01)
			k = 160.0f / sqrt(K);
		else if (K < -0.01)
			k = 160.0f / sqrt(-1.0f * K);

		float object_1_r = position.r / k;
		float object_2_r = other_position.r / k;

		if (K > 0)
		{
			float cos_dist = cos(object_1_r) * cos(object_2_r) + sin(object_1_r) * sin(object_2_r) * cos_delta_theta;
			return acos(cos_dist) * k;
		}
		else
		{
			float cosh_dist = cosh(object_1_r) * cosh(object_2_r) - sinh(object_1_r) * sinh(object_2_r) * cos_delta_theta;
			return acosh(cosh_dist) * k;
		}
	}
}
