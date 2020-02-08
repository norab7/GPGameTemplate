// Credit to: LearnOpenGL.com

#include "Model.h"

using namespace _Model;

Model::Model(const char* id, const char* path) {
	this->id = id;
	load(path);
}

void Model::load(const std::string& path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	dir = path.substr(0, path.find_last_of('/'));
	process_node(scene->mRootNode, scene);
	meshes.insert(meshes.begin(), process_bounds_mesh());
}

void Model::Draw(Shader& shader, bool show_bounds) {
	if(show_bounds) { meshes[0].Draw(shader, true); }

	for(unsigned int i = 1; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void Model::process_node(aiNode* node, const aiScene* scene) {
	// Parent nodes
	unsigned int i = 0;
	for(i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}

	// Child nodes, recursively process all nodes and meshes.
	for(i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	glm::vec4 average(0);

	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// Vertex positions, Normals, Textures-coords
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.TexCoords = (mesh->mTextureCoords[0]) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0, 0);

		// Check for bounding area
		check_min_max(vertex.Position);
		average = glm::vec4(vertex.Position + vertex.Position, ++average[3]);

		// Push the vertex to the vector list
		vertices.push_back(vertex);
	}

	// Indices; Each mesh has faces, each face has indices for each vertex
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// TODO: process material
	if(mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Get and insert all diffuse textures into the texture vector
		std::vector<Texture> diffuse_map = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_map.begin(), diffuse_map.end());

		// Get and insert all specular textures into the texture vector
		std::vector<Texture> specular_map = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_map.begin(), specular_map.end());
	}

	// Get the centre average of the shape
	average = glm::vec4(average.x / average.w, average.y / average.w, average.z / average.w, average.w);

	return Mesh(vertices, indices, textures, average);
}

void Model::check_min_max(glm::vec3 vertex) {
	// Set min bounds
	if(bounds_min.x > vertex.x) { bounds_min.x = vertex.x; }
	if(bounds_min.y > vertex.y) { bounds_min.y = vertex.y; }
	if(bounds_min.z > vertex.z) { bounds_min.z = vertex.z; }

	// Set max bounds
	if(bounds_max.x < vertex.x) { bounds_max.x = vertex.x; }
	if(bounds_max.y < vertex.y) { bounds_max.y = vertex.y; }
	if(bounds_max.z < vertex.z) { bounds_max.z = vertex.z; }

}

Mesh Model::process_bounds_mesh() {
	std::vector<glm::vec3> vertices;
	//std::vector<unsigned int> indices {1,2,3, 1,2,4, 1,3,4,   7,2,4, 7,2,5, 7,4,5,	6,3,4, 6,3,5, 6,4,5,	8,2,3, 8,2,5, 8,3,5}; // Manual Calculation based on positions of vertices
	std::vector<unsigned int> indices {7,2,4,7,1,2,7,1,4,	6,1,3,6,1,4,6,3,4,	5,2,4,5,4,3,5,3,2,	0,1,3,0,3,2,0,2,1}; // Manual Calculation based on positions of vertices

	std::vector<Texture> textures;
	glm::vec4 average(0);

	// Get top of bounding box																					// Indices for faces
	vertices.push_back(bounds_max);																				// 1
	vertices.push_back(glm::vec3(bounds_max.x - (bounds_max.x - bounds_min.x), bounds_max.y, bounds_max.z));	// 2
	vertices.push_back(glm::vec3(bounds_max.x, bounds_max.y - (bounds_max.y - bounds_min.y), bounds_max.z));	// 3
	vertices.push_back(glm::vec3(bounds_max.x, bounds_max.y, bounds_max.z - (bounds_max.z - bounds_min.z)));	// 4

	// Get bottom of bounding box
	vertices.push_back(bounds_min);																				// 5
	vertices.push_back(glm::vec3(bounds_min.x + (bounds_max.x - bounds_min.x), bounds_min.y, bounds_min.z));	// 6
	vertices.push_back(glm::vec3(bounds_min.x, bounds_min.y + (bounds_max.y - bounds_min.y), bounds_min.z));	// 7
	vertices.push_back(glm::vec3(bounds_min.x, bounds_min.y, bounds_min.z + (bounds_max.z - bounds_min.z)));	// 8

	std::vector<Vertex> vertex_vector;
	for(unsigned int i = 0; i < vertices.size(); i++) {
		Vertex vertex;
		vertex.Position = vertices[i];
		vertex.Normal = vertices[i];

		average = glm::vec4(vertex.Position + vertex.Position, ++average[3]);
		vertex_vector.push_back(vertex);
	}

	average = glm::vec4(average.x / average.w, average.y / average.w, average.z / average.w, average.w);
	return Mesh(vertex_vector, indices, textures, average);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string name) {
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for(unsigned int j = 0; j < loaded_textures.size(); j++) {
			if(std::strcmp(loaded_textures[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}
		if(!skip) {
			Texture texture;
			texture.id = texture_from_file(str.C_Str(), dir);
			texture.type = type;
			texture.path = str.C_Str();
			textures.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::texture_from_file(const char* path, const std::string& dir, bool gamma) {
	std::string filename = dir + '/' + std::string(path);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// Load and store the texture in a char array
	int width, height, components;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 0);
	if(!data) {
		// if loading from obj data does not work, try loading direct from obj location
		filename = dir + '/' + std::string(path).substr(std::string(path).find_last_of("/\\") + 1);
		data = stbi_load(filename.c_str(), &width, &height, &components, 0);

		if(!data) {
			// If there is still no texture file found, just throw an error
			std::cout << "Model::texture_from_file: Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
			return textureID;
		}
	}

	GLenum format;

	// Check the type of texture then set the format
	if(components == 1) {
		format = GL_RED;
	} else if(components == 3) {
		format = GL_RGB;
	} else if(components == 4) {
		format = GL_RGBA;
	}

	// Standard binding of textureID, and size information
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// How the texture will be displayed in different situations like wrapping etc
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Free up the space used
	stbi_image_free(data);

	// return the textureID as it is bound and can be referred to through its ID
	return textureID;
}

const char* Model::get_id() {
	return this->id;
}

void Model::randomize_mesh_vertices() {
	for(Mesh m : meshes) {
		m.randomize_vertices();
	}
}