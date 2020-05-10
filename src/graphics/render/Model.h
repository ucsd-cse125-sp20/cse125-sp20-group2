#pragma once

// Components
#include <graphics/render/Shader.h>
#include <graphics/render/Mesh.h>

// OpenGL
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Textures
#include <SOIL/SOIL.h>

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Std library
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:

	// Model data
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  Functions   */
	
	Model(std::string const &path, bool gamma = false);

	// Draw all the meshes in the file
	void draw(Shader shader);

	/// Sizing (TODO: modelHeight unused)
	float modelWidth, modelHeight, modelDepth;

private:
	/*  Functions   */
	void loadModel(std::string path);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};