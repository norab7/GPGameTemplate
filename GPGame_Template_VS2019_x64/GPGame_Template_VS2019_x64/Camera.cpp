#include "Camera.h"

using namespace _Camera;

// TODO: Remove camera string
Camera::Camera(glm::vec3 pos) : Entity::Entity("Camera", pos) {
	set_target(glm::vec3(pos.x, pos.y, pos.z-1));
}

void Camera::set_target(glm::vec3 target, glm::vec3 world_up) {
	this->target = target;
	this->direction = glm::normalize(this->position - this->target);
	this->right = glm::normalize(glm::cross(world_up, direction));
	this->up = glm::normalize(glm::cross(direction, right));
	this->front = glm::normalize(-direction);
}

glm::mat4& Camera::look_at() {
	view = glm::lookAt(position, position + front, up);
	return view;
}

void Camera::update_vectors() {
	glm::vec3 worldup = glm::vec3(0, 1, 0);

	// looking at directions
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	// standard directions
	right = glm::normalize(glm::cross(front, worldup));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}