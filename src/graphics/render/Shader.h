#pragma once

// OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// the shader program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	void use();
	
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set4f(const std::string& name, float x, float y, float z, float w) const;
	void set4f(const std::string& name, glm::vec4 vec) const;
	void setMat4(const std::string& name, glm::mat4 mat) const;
	void setMat3(const std::string& name, glm::mat3 mat) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;

private:
	// Prints out the compile error if shader compilation was unsuccessful.
	void checkCompileErrors(unsigned int shader, std::string type);
};
