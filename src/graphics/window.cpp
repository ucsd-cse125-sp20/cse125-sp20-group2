
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
#include <graphics/_options/graphics_vars.h>
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
	this->isClosed = false;
	this->width = width;
	this->height = height;
	this->setupWindow();

	// FIXME - HARDCODED ABSOLUTE PATH
	this->shader = new Shader("C:\\Users\\JQ124\\Desktop\\CompSci\\cse125\\cse125-sp20-group2\\src\\graphics\\shaders\\vert_shader.glsl", "C:\\Users\\JQ124\\Desktop\\CompSci\\cse125\\cse125-sp20-group2\\src\\graphics\\shaders\\frag_shader.glsl");
}

void Window::addObject(unsigned int id, GameObject object) {
	this->objectsToRender.insert({id, object});
	objNum++;
}

void Window::removeObject(unsigned int index) {
	this->objectsToRender.erase(index);
	objNum--;
}

void Window::setupWindow() {
	// // //
	// Setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		
	GLFWwindow* window = glfwCreateWindow((int)WIN_WIDTH, (int)WIN_HEIGHT, "Comrade's Kitchen", NULL, NULL);

	// Capture mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Mouse centering
	glfwSetCursorPos(window, lastX, lastY);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Current context is window
	glfwMakeContextCurrent(window);

	// Capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

	// Register callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Enable depth testing
	// This is used to make sure we occlude textures when a
	// surface is obscured by another surface (i.e. in 3D objects).
	glEnable(GL_DEPTH_TEST);

	// Set window var
	this->window = window;


	// FIXME: GRID STUFF
	GameObject grid = GameObject("C:\\Users\\JQ124\\Desktop\\CompSci\\cse125\\cse125-sp20-group2\\assets\\models\\grid_square.obj", glm::vec3(0.5, -1.25, 0.5), 0.2);
	this->addObject(0, grid);
	objNum++;
}

void Window::close() {
	if (glfwWindowShouldClose(window))
	{
		isClosed = true; 
		glfwTerminate();
	}
	else std::cerr << "UNABLE TO CLOSE WINDOW" << std::endl;
}

void Window::render()
{
	if (window == NULL) {
		std::cerr << "ERROR: No window!" << std::endl;
	}

	// // //
	// User shader program
	shader->use();

	// // //
	// The render operations

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
	shader->setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0f));
	shader->setFloat("ambientStrength", ambientStrength);
	shader->setVec3("lightPos", lightPos);
	shader->setVec3("defaultObjColor", defaultObjColor);
	shader->setFloat("noColorPrecision", noColorPrecision);
	shader->setFloat("specularStrength", specularStrength);
	shader->setVec3("viewPos", cam.pos);
	
	// // //
	// View + perspective projection matrices

	// camera view transformation (related to xy axis rotations)
	glm::mat4 view = cam.getViewMatrix();

	// camera projection transformation (related to zooms)
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(cam.zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

	// pass transformation matrices to the shader
	shader->setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader->setMat4("view", view);

	// // //
	// Render each GameObject

	for (auto it = objectsToRender.begin(); it != objectsToRender.end(); ++it) {
		GameObject obj = it->second;

		// Respective model matrix of each object
		shader->setMat4("model", obj.modelMatrix);

		// Used to convert normal vectors to world space coordinates, without applying translations to them
		glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(obj.modelMatrix)));
		shader->setMat4("normalMatrix", normalMatrix);

		// Draw the model
		obj.draw(*shader);
	}

	// // //
	// GLFW stuff
	glfwSwapBuffers(window);
	glfwPollEvents();

	// TEMP FIXME
	if (glfwWindowShouldClose(window)) close();
}

// Handle user input
void processInput(GLFWwindow* window)
{
	// Exit application.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Camera movement (depends on framerate)
	cam.moveSpeed = 50 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam.processKeyMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam.processKeyMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.processKeyMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
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