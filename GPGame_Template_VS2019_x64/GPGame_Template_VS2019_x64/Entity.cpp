#include "Entity.h"

using namespace _Entity;

Entity::Entity(const char* id, glm::vec3 pos) {
	this->id = id;
	this->position = pos;
	matrix = glm::translate(glm::mat4(1.0f), pos);
}

bool Entity::isEqual(Entity e) {
	return strcmp(this->id, e.id) == 0;
}


void Entity::add_child(Entity child) {
	child.child = true;
	children.push_back(child);
	// TODO: match up vectors, parent to child
}

// Remove all children matching the given child
void Entity::remove_child(Entity child, bool deep) {
	std::vector<Entity>::iterator it;
	it = children.begin();
	for(int i = 0; i < children.size(); i++) {
		if(deep) { children.at(i).remove_child(child, deep); }
		if(children.at(i).isEqual(child)) { children.erase(it); }
		it++;
	}
}

// Removes all children in all current and child nodes
void Entity::remove_children() {
	for(Entity e : children) {
		e.remove_children();
	}
	children.clear();
}

glm::mat4 Entity::get_matrix() {
	return this->matrix;
}

glm::vec3 Entity::get_position() {
	return position;
}

void Entity::set_position(glm::vec3 pos) {
	this->position = pos;
	matrix[0][3] = position.x;
	matrix[1][3] = position.y;
	matrix[2][3] = position.z;
}

void Entity::transform(glm::mat4 trans) {
	matrix *= trans;
	for(Entity e : children) { e.transform(trans); }
}

void Entity::translate(glm::vec3 trans) {
	position *= trans;
	for(Entity e : children) { e.translate(trans); }
}

void Entity::rotate(float degrees, glm::vec3 axis) {
	matrix = glm::rotate(matrix, glm::radians(degrees), axis);
	for(Entity e : children) { e.rotate(degrees, axis); }
}

void Entity::scale(float scale) {
	matrix = glm::scale(matrix, glm::vec3(scale));
	for(Entity e : children) { e.scale(scale); }
}

void Entity::scale(glm::vec3 scale) {
	matrix = glm::scale(matrix, scale);
	for(Entity e : children) { e.scale(scale); }
}

Entity::~Entity() {

}