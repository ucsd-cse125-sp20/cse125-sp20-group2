
#include <glad/glad.h>	// Include this...
#include <GLFW/glfw3.h> // ...before this.
#include <iostream>
#include <assimp/config.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.cpp"
#include "Shader.h"
#include "Camera.cpp"
#include "Model.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Camera + lookat vectors + euler angles
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 pos = glm::vec3(0, 0, 3);
glm::vec3 up = glm::vec3(0, 1, 0);
glm::vec3 front = glm::vec3(0, 0, -1);
//float yaw = -90.0f;
//float pitch = 0.0f;

// Width and height (TEMP - FIXME)
const float WIN_WIDTH = 1800;
const float WIN_HEIGHT = 1600;

// Initial mouse positions
float lastX = WIN_WIDTH / 2, lastY = WIN_HEIGHT / 2;
const float RUN_SPEED = 1, TURN_SPEED = 1;
float currentSpeed = 0.0f, currentTurnSpeed = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 modelPos;
float modelRotationX;

int main()
{
	// // //
	// Setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow *window = glfwCreateWindow((int)WIN_WIDTH, (int)WIN_HEIGHT, "LearnOpenGL", NULL, NULL);

	// Capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Mouse centering
	glfwSetCursorPos(window, lastX, lastY);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Current context is window
	glfwMakeContextCurrent(window);

	// Register callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Enable depth testing
	// This is used to make sure we occlude textures when a
	// surface is obscured by another surface (i.e. in 3D objects).
	glEnable(GL_DEPTH_TEST);

	// // //
	// Models
	Model ourModel("../../../assets/models/Basic_Character_Model.obj");

	// // //
	// Shaders

	// Shader definition
	Shader shader = Shader("shaders/vert_shader.glsl", "shaders/frag_shader.glsl");
	shader.use();

	// // //
	// The render loop
	while (!glfwWindowShouldClose(window))
	{
		// // //
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// // //
		// Input
		processInput(window);

		// // //
		// Rendering stuff

		// Make BG light gray
		glClearColor(0.8f, 0.8f, 0.8f, 0.8f);

		// Clear color + depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// // //
		// View + perspective projection matrices

		// camera view transformation (related to xy axis rotations)
		glm::mat4 view = cam.getViewMatrix();

		// camera projection transformation (related to zooms)
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(cam.zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

		// pass transformation matrices to the shader
		shader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		shader.setMat4("view", view);

		// Calculate distance and rotation
		modelRotationX += currentTurnSpeed * deltaTime;
		float distance = currentSpeed * deltaTime;
		float dx = distance * sin(modelRotationX);
		float dz = distance * cos(modelRotationX );
		modelPos.x += dx;
		modelPos.z += dz;

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, modelPos); // translate it down so it's at the center of the scene
		model = glm::rotate(model, modelRotationX , glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		
		shader.setMat4("model", model);
		ourModel.draw(shader);

		// // //
		// GLFW stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Call terminate to cleanup GLFW resources.
	glfwTerminate();
	return 0;
}

// Handle user input
void processInput(GLFWwindow *window)
{
	// Exit application.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cam.toggleFreeCam();

	float velocity = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		currentSpeed = RUN_SPEED;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		currentSpeed = -RUN_SPEED;
	else
		currentSpeed = 0;
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		currentTurnSpeed = -TURN_SPEED;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		currentTurnSpeed = TURN_SPEED;
	else
		currentTurnSpeed = 0;

	
}

// A callback function to resize the rendering window when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

	// Calculate offset based on previous position of the mouse
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	cam.processMouseMovement(xoffset, yoffset);
}

// Zoom callback for when mouse wheel is scrolled
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	cam.processMouseScroll(yoffset);
}