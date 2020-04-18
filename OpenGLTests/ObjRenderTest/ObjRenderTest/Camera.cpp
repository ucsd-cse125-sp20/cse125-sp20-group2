#pragma once

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
const float INIT_PITCH = 0.0f;

// Default camera movement options
const float INIT_SPEED = 2.5f;
const float INIT_SENSITIVITY = 0.1f;
const float INIT_ZOOM = 45.0f;

// Default lookAt vectors
const glm::vec3 INIT_POS = glm::vec3(0, 0, 3);
const glm::vec3 INIT_UP = glm::vec3(0, 1, 0);
const glm::vec3 INIT_FRONT = glm::vec3(0, 0, -1);

class Camera {
public:
	// Camera Attributes
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

	// Constructor with vectors (takes in pos, up, euler angle vectors, and
	// uses default front, movement options)
	Camera(glm::vec3 pos = INIT_POS, glm::vec3 up = INIT_UP, float yaw = INIT_YAW, float pitch = INIT_PITCH, bool freeCam = true) 
		: front(INIT_FRONT), moveSpeed(INIT_SPEED), sensitivity(INIT_SENSITIVITY), zoom(INIT_ZOOM)
	{
		this->pos = pos;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->freeCam = freeCam;
		updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, bool freeCam = true)
		: front(INIT_FRONT), moveSpeed(INIT_SPEED), sensitivity(INIT_SENSITIVITY), zoom(INIT_ZOOM)
	{
		pos = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->freeCam = freeCam;
		updateCameraVectors();
	}

	void toggleFreeCam() {
		freeCam = !freeCam;
		if (!freeCam) {

			// FIXME - hard-coded reset, should target player
			pos = glm::vec3(0, 2, 2);
			front = glm::vec3(0, -0.5, 0);
			pitch = -45;
			yaw = -90;
			updateCameraVectors();
		}
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(pos, pos + front, up);
	}

	// Set camera target

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyMovement(Camera_Movement direction, float deltaTime)
	{
		if (!freeCam) return;

		float velocity = moveSpeed * deltaTime;
		if (direction == FORWARD)
			pos += front * velocity;
		if (direction == BACKWARD)
			pos -= front * velocity;
		if (direction == LEFT)
			pos -= right * velocity;
		if (direction == RIGHT)
			pos += right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset)
	{
		if (!freeCam) return;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		// Update front, right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset)
	{
		if (!freeCam) return;

		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new front vector
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(newFront);
		// Also re-calculate the right and Up vector
		right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, front));
	}
};