#pragma once

#include <util/Config.h>
#include <objects/GameObject.h>

// Std. library
#include <iostream>

// OpenGL stuff
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// Defines several possible options for camera movement.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default euler angles
const float INIT_YAW = -90.0f;
const float INIT_PITCH = -45.0f;

// Default camera movement options
const float INIT_SENSITIVITY = 0.1f;
const float INIT_ZOOM = 45.0f;

// Default lookAt vectors (if not given)
const glm::vec3 INIT_POS = glm::vec3(0, 0, 0);
const glm::vec3 INIT_UP = glm::vec3(0, 1, 0);
const glm::vec3 INIT_FRONT = glm::vec3(0, 0, -1);

class Camera {
public:

	// Camera Attributes
	glm::vec3 staticPos;   // Refers to the position held when freecam is disabled.
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Control booleans
	bool freeCam;

	// Camera angles
	float yaw;
	float pitch;

	// Movement options
	float moveSpeed;
	float sensitivity;
	float zoom;

	// Camera target (if freecam is disabled)
	GameObject* target;

	// Constructor with vector values
	Camera(glm::vec3 pos = INIT_POS, glm::vec3 up = INIT_UP, float yaw = INIT_YAW, float pitch = INIT_PITCH, bool freeCam = true);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, bool freeCam = true);

	void setTarget(GameObject* target);
	GameObject* getTarget();

	void toggleFreeCam();

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyMovement(Camera_Movement direction);

	void warpToTarget();

	void processMouseMovement(float xoffset, float yoffset);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};