#pragma once

#include "Model.h"
#include "Entity.h"

namespace _GameObject {
	class GameObject : public _Entity::Entity, public _Model::Model {

	public:
		GameObject(const char* id, const char* dir);
		GameObject(const char* id, const char* dir, glm::vec3 position);
		~GameObject();
	};
}
