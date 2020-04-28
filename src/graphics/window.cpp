
#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.
#include <iostream>
#include <assimp/config.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/_obj/Mesh.h>
#include <graphics/_obj/Shader.h>
#include "Camera.cpp"
#include "Model.cpp"
#include "GameObject.cpp"
#include "_options/graphics_vars.h"
#include "window.h"

//// Camera at desiginated position
Camera cam(INITIAL_CAM_POS);

// Initial mouse positions
float lastX = WIN_WIDTH / 2, lastY = WIN_HEIGHT / 2;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window::Window(int width = WIN_WIDTH, int height = WIN_HEIGHT) : objNum (0) {
	this->width = width;
	this->height = height;
}

bool Window::addObject(unsigned int id, GameObject object) {
	this->objectsToRender[id] = object;
}

bool Window::removeObject(unsigned int index) {
	this->objectsToRender.erase(index);
}

int Window::render()
{
	// // //
	// Setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		
	GLFWwindow* window = glfwCreateWindow((int)WIN_WIDTH, (int)WIN_HEIGHT, "LearnOpenGL", NULL, NULL);

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
	// Shaders

	// Shader definition
	Shader shader = Shader("C:\\Users\\wix003\\Documents\\Project\\cse125\\src\\graphics\\shaders\\vert_shader.glsl", "C:\\Users\\wix003\\Documents\\Project\\cse125\\src\\graphics\\shaders\\frag_shader.glsl");
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

		// Apply lighting options
		shader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0f));
		shader.setFloat("ambientStrength", ambientStrength);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("defaultObjColor", defaultObjColor);
		shader.setFloat("noColorPrecision", noColorPrecision);
		shader.setFloat("specularStrength", specularStrength);
		shader.setVec3("viewPos", cam.pos);
		
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

		// // //
		// Render each GameObject

		// Model matrix of each object, with its respective model matrix
		shader.setMat4("model", nanosuit.modelMatrix);

		// Used to convert normal vectors to world space coordinates, without applying translations to them
		glm::mat4 normalMatrix = glm::mat3(transpose(inverse(nanosuit.modelMatrix)));
		shader.setMat4("normalMatrix", normalMatrix);

		// Draw the model
		nanosuit.draw(shader);

		// GameObject-specific code goes above
		// // //

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
void processInput(GLFWwindow* window)
{
	// Exit application.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Camera movement (depends on framerate)
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.processKeyMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.processKeyMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.processKeyMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.processKeyMovement(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cam.toggleFreeCam();
}

// A callback function to resize the rendering window when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	// Calculate offset based on previous position of the mouse
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	cam.processMouseMovement(xoffset, yoffset);
}

// Zoom callback for when mouse wheel is scrolled
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.processMouseScroll(yoffset);
}

//// int main() {
//// 	return render();
//// }