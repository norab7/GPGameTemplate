// Credit to: LearnOpenGL.com

#pragma once

#include "Entity.h"
#include "Shader.h"

#include <string>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

namespace _Mesh {
	class Mesh {
		unsigned int VAO, VBO, EBO;

		void setup_mesh();

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
		void Draw(Shader& shader);

		void randomize_vertices();

	};
}