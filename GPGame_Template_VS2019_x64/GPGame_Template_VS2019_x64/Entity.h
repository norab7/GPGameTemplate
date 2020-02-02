#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace _Entity {
	class Entity {
		const char* id;
		bool child = false;

	protected:
		glm::mat4 matrix;
		glm::vec3 position;

		std::vector<Entity> children;
	public:
		Entity(const char* id, glm::vec3 pos = glm::vec3(0, 0, 0));

		bool isEqual(Entity e);

		void add_child(Entity child);
		void remove_child(Entity child, bool deep = false);
		void remove_children();

		glm::vec3 get_position();

		glm::mat4 get_matrix();
		void set_position(glm::vec3 pos);
		void transform(glm::mat4 trans);
		void translate(glm::vec3 trans);
		void rotate(float degrees, glm::vec3 axis);
		void scale(float scale);
		void scale(glm::vec3 scale);

		~Entity();
	};
}