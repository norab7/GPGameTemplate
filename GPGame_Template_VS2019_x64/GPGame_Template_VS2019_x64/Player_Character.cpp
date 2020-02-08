#include "Player_Character.h"
using namespace _Player_Character;

Player_Character::Player_Character() : _Camera::Camera() {

}

void Player_Character::process_keyboard(bool* KEYS) {
	// TODO: figure out why adding deltaTime causes huge lag
	float deltaSpeed = speed; //  *delta;
	if(KEYS[GLFW_KEY_LEFT_SHIFT]) { deltaSpeed *= 2; }
	if(KEYS[GLFW_KEY_LEFT_ALT]) { deltaSpeed /= 2; }
	if(KEYS[GLFW_KEY_W]) { set_position(get_position() + front * deltaSpeed); }
	if(KEYS[GLFW_KEY_S]) { set_position(get_position() - front * deltaSpeed); }
	if(KEYS[GLFW_KEY_A]) { set_position(get_position() - right * deltaSpeed); }
	if(KEYS[GLFW_KEY_D]) { set_position(get_position() + right * deltaSpeed); }
	if(KEYS[GLFW_KEY_SPACE]) { set_position(get_position() + up * deltaSpeed); }
	if(KEYS[GLFW_KEY_LEFT_CONTROL]) { set_position(get_position() - up * deltaSpeed); }
}

void Player_Character::process_mouse_movement(double xpos, double ypos) {
	yaw += xpos * sensitivity;
	pitch += ypos * sensitivity;

	if(pitch > 89.0f) { pitch = 89.0f; }
	if(pitch < -89.0f) { pitch = -89.0f; }

	update_vectors();
}

void Player_Character::process_mouse_scroll() {

}

Player_Character::~Player_Character() {

}
