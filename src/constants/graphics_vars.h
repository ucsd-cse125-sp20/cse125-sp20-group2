#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float ambientStrength = 0.5f;
const glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
const glm::vec3 defaultObjColor = glm::vec3(0.2f, 0.4f, 0.7f);
const float noColorPrecision = 0.01;
const float specularStrength = 0.9;

// Other
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);