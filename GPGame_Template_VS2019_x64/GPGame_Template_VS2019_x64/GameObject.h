#pragma once

#include "Model.h"
#include "Entity.h"
#include "I_AABB.h"

namespace _GameObject {
	class GameObject : public _Entity::Entity, public _Model::Model, public I_AABB {

	public:
		GameObject(const char* id, const char* dir);
		GameObject(const char* id, const char* dir, glm::vec3 position);
		~GameObject();

		AABB get_AABB() const override;
	};
}
