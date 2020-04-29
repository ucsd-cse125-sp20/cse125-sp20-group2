#pragma once

#include "_obj/Shader.h"
#include "Model.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "_options/graphics_vars.h"

class GameObject {
public:
	Model model;
	glm::vec3 worldPos;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 scaleVec;
	int ID;

	GameObject(std::string modelPath, glm::vec3 worldPos, float uniformScale) : model(modelPath) {
		moveTo(worldPos);
		applyScale(glm::vec3(uniformScale));
	}
	GameObject(std::string modelPath, glm::vec3 worldPos, glm::vec3 scaleVec) : model(modelPath) {
		moveTo(worldPos);
		applyScale(scaleVec);
	}
	GameObject(std::string modelPath, float x, float y, float z, float uniformScale) : model(modelPath) {
		moveTo(glm::vec3(x, y, z));
		applyScale(glm::vec3(uniformScale));
	}
	GameObject(std::string modelPath, float x, float y, float z, float sX, float sY, float sZ) : model(modelPath) {
		moveTo(glm::vec3(x, y, z));
		applyScale(glm::vec3(sX, sY, sZ));
	}

	void draw(Shader shader) {
		model.draw(shader);
	}

	void setID(int ID) {
		this->ID = ID;
	}
	
private:

	// Update the world position and move the model matrix
	void moveTo(glm::vec3 loc) {
		worldPos = loc;
		modelMatrix = glm::translate(modelMatrix, loc);
	}

	// Apply scaling to the model.
	void applyScale(glm::vec3 scale) {
		scaleVec = scale;
		modelMatrix = glm::scale(modelMatrix, scale);
	}

	// Rotate the model.
	void rotate(float rotation) {
		modelMatrix = glm::rotate(modelMatrix, rotation, UP);
	}
};