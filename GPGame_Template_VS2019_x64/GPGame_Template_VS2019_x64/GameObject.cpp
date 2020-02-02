#include "GameObject.h"

using namespace _GameObject;

GameObject::GameObject(const char* id, const char* dir) : Entity(id), Model(id, dir) {

}

GameObject::GameObject(const char* id, const char* dir, glm::vec3 position) : Entity(id, position), Model(id, dir) {

}

GameObject::~GameObject() {

}