#pragma once
#include <iostream>
#include "Entity.h"
#include "I_Input.h"

namespace _Camera {
	class Camera : public _Entity::Entity {


	protected:
		glm::vec3 target;
		glm::vec3 direction;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 front;

		glm::mat4 view;

		float yaw = -90.0f;
		float pitch = 0.0f;

		const float speed = 0.05f;

		void update_vectors();
	public:
		Camera(glm::vec3 pos = glm::vec3(0, 0, 0));
		~Camera();

		glm::mat4& look_at();

		void set_target(glm::vec3 target, glm::vec3 world_up = glm::vec3(0, 1, 0));

	};
}