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
}

void Model::Draw(Shader& shader) {
	for(unsigned int i = 0; i < meshes.size(); i++) {
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

	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// Vertex positions, Normals, Textures-coords
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.TexCoords = (mesh->mTextureCoords[0]) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0, 0);

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
	return Mesh(vertices, indices, textures);
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