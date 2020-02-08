#include "GameObject.h"

using namespace _GameObject;

GameObject::GameObject(const char* id, const char* dir) : Entity(id), Model(id, dir) {

}

GameObject::GameObject(const char* id, const char* dir, glm::vec3 position) : Entity(id, position), Model(id, dir) {

}

GameObject::~GameObject() {

}

AABB GameObject::get_AABB() const {

	const glm::vec3 position = get_position();
	//std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ") ";

	//return *new AABB(bounds_min.x, bounds_min.y, bounds_min.z, bounds_max.x, bounds_max.y, bounds_max.z);
	return *new AABB(bounds_min.x + position.x, bounds_min.y + position.y, bounds_min.z + position.z, bounds_max.x +position.x, bounds_max.y + position.y, bounds_max.z + position.z);
}