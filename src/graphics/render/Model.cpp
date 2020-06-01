// Components
#include <graphics/render/Model.h>
	
Model::Model(std::string const &path, bool gamma) : gammaCorrection(gamma)
{
	loadModel(path);
}

// Draw all the meshes in the file
void Model::draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader);
}

/*  Functions   */
void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	/// NOTE: Debug
	//std::cout << std::endl << "Model: " << path << std::endl;

	processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Min/Max for calculating width/depth/height
	/// NOTE: Height is unused until we need vertical collisions.
	float minX = 9999.9, minY = 9999.9, minZ = 9999.9;
	float maxX = -9999.9, maxY = -9999.9, maxZ = -9999.9; 

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		// Get min/maxes of coords
		if (vector.x < minX) minX = vector.x;
		else if (vector.x > maxX) maxX = vector.x;
		if (vector.y < minY) minY = vector.y;
		else if (vector.y > maxY) maxY = vector.y;
		if (vector.z < minZ) minZ = vector.z;
		else if (vector.z > maxZ) maxZ = vector.z;

		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.norm = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		} else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	/// NOTE: This is no longer used in actual collision calculations. Please use this to update model sizes in the config file.
	/// Can be removed in the final release.
	modelWidth = maxX - minX;
	modelDepth = maxZ - minZ;
	modelHeight = maxY - minY;

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	/// NOTE: Debug statements to enumerate textures
	/*std::cout << "Listing textures for this object: " << std::endl;
	for (auto tex : textures) {
		std::cout << "Texture " << tex.path << " of type " << tex.type << std::endl;
	}
    std::cout << "Texture count for this object: " << textures.size() << std::endl;*/

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	/// NOTE: Debug
	//std::cout << "Using directory:  " << directory << std::endl;
	// Debug
	if (std::string(path) == std::string("") || path == NULL) std::cerr << "Texture loading error: No texture path given!" << std::endl;

	//Generate texture ID and load texture data
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);

	if (textureID == 0)
	{
		std::cerr << "Texture loading error: Trying to load texture when OpenGL context not set!" << std::endl;
		return 0;
	}

	int width, height;

	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	
	if (image == NULL)  {
		std::cerr << "======" << std::endl;
		std::cerr << "Model loading error: Texture image not found!" << std::endl;
		std::cerr << "Path given:" << std::endl;
		std::cerr << std::string(filename) << std::endl;
		std::cerr << "======" << std::endl;
		std::cerr << "Error: " << std::endl;
		std::cerr << std::string(SOIL_last_result()) << std::endl;
		std::cerr << "======" << std::endl;
	}

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	/// NOTE: Debug
	//std::cout << "Material: " << std::string(mat->GetName().C_Str()) << std::endl;
	//std::cout << "Texture count: " << mat->GetTextureCount(type) << std::endl;
	//std::cout << "Texture type: " << typeName << std::endl;

	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}