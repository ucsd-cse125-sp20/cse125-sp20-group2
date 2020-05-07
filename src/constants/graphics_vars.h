#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float ambientStrength = 0.5f;
const glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
const glm::vec3 defaultObjColor = glm::vec3(0.2f, 0.4f, 0.7f);
const float noColorPrecision = 0.01;
const float specularStrength = 0.9;

// Window constants
const float WIN_WIDTH = 1800;
const float WIN_HEIGHT = 1600;

// Initial camera position
const glm::vec3 INITIAL_CAM_POS = glm::vec3(0, 7, 7);

// Other
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);