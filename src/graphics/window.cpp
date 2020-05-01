
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

// Player movement options (FIXME)
const float RUN_SPEED = 1, TURN_SPEED = 1;
float currentSpeed = 0.0f, currentTurnSpeed = 0.0f;
float modelRotationX;
glm::vec3 modelPos;

// Initial mouse positions
float lastX = WIN_WIDTH / 2, lastY = WIN_HEIGHT / 2;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* glfwViewport, int width, int height);
void processInput(GLFWwindow* glfwViewport);
void mouse_callback(GLFWwindow* glfwViewport, double xpos, double ypos);
void scroll_callback(GLFWwindow* glfwViewport, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window::Window(int width = WIN_WIDTH, int height = WIN_HEIGHT) : objNum (0) {
	this->isClosed = false;
	this->width = width;
	this->height = height;
	this->setupWindow();
	this->shader = new Shader("src\\graphics\\shaders\\vert_shader.glsl", "src\\graphics\\shaders\\frag_shader.glsl");
}

void Window::addObject(unsigned int id, GameObject* object) {
	this->objectsToRender.insert({id, object});
	objNum++;
}

void Window::removeObject(unsigned int index) {
	this->objectsToRender.erase(index);
	objNum--;
}

void Window::setupWindow() {

	modelPos = glm::vec3(0);
	// // //
	// Setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		
	GLFWwindow* glfwViewport = glfwCreateWindow((int)WIN_WIDTH, (int)WIN_HEIGHT, "Comrade's Kitchen", NULL, NULL);

	// Capture mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Mouse centering
	glfwSetCursorPos(glfwViewport, lastX, lastY);

	if (glfwViewport == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Current context is window
	glfwMakeContextCurrent(glfwViewport);

	// Capture mouse
	glfwSetInputMode(glfwViewport, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

	// Register callback functions
	glfwSetFramebufferSizeCallback(glfwViewport, framebuffer_size_callback);
	glfwSetCursorPosCallback(glfwViewport, mouse_callback);
	glfwSetScrollCallback(glfwViewport, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
	}

	// Enable depth testing
	// This is used to make sure we occlude textures when a
	// surface is obscured by another surface (i.e. in 3D objects).
	glEnable(GL_DEPTH_TEST);

	// Set window var
	this->glfwViewport = glfwViewport;


	// FIXME: GRID STUFF
	GameObject* grid = new GameObject("assets\\models\\grid_square.obj", glm::vec3(0.5, -1.25, 0.5), 0.2);
	this->addObject(0, grid);
	objNum++;
}

void Window::close() {
	if (glfwWindowShouldClose(glfwViewport))
	{
		isClosed = true; 
		glfwTerminate();
	}
	else std::cerr << "ERROR: Unable to close window!" << std::endl;
}

void Window::render()
{
	if (glfwViewport == NULL) {
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
	//processInput(glfwViewport);

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

	// FIXME - MOVE TO SERVER IN THE FUTURE
	// Calculate distance and rotation
	modelRotationX += currentTurnSpeed * deltaTime;
	float distance = currentSpeed * deltaTime;
	float dx = distance * sin(modelRotationX);
	float dz = distance * cos(modelRotationX);
	modelPos.x += dx;
	modelPos.z += dz;
	player->modelMatrix = glm::mat4(1.0f);
	player->modelMatrix = glm::translate(player->modelMatrix, modelPos); // translate it down so it's at the center of the scene
	player->modelMatrix = glm::rotate(player->modelMatrix, modelRotationX , glm::vec3(0.0f, 1.0f, 0.0f));
	player->modelMatrix = glm::scale(player->modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

	// // //
	// Render each GameObject

	for (auto it = objectsToRender.begin(); it != objectsToRender.end(); ++it) {
		GameObject* obj = it->second;

		// Respective model matrix of each object
		shader->setMat4("model", obj->modelMatrix);

		// Used to convert normal vectors to world space coordinates, without applying translations to them
		glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(obj->modelMatrix)));
		shader->setMat4("normalMatrix", normalMatrix);

		// Draw the model
		obj->draw(*shader);
	}

	// // //
	// GLFW stuff
	glfwSwapBuffers(glfwViewport);
	glfwPollEvents();

	// Close the window when appropriate.
	if (glfwWindowShouldClose(glfwViewport)) close();
}

// Handle user input
void Window::processInput()
{
	// Exit application.
	if (glfwGetKey(glfwViewport, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(glfwViewport, true);
	}

	// Camera movement (depends on framerate)
	cam.moveSpeed = 50 * deltaTime;
	if (glfwGetKey(glfwViewport, GLFW_KEY_UP) == GLFW_PRESS)
		cam.processKeyMovement(FORWARD, deltaTime);
    if (glfwGetKey(glfwViewport, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam.processKeyMovement(BACKWARD, deltaTime);
    if (glfwGetKey(glfwViewport, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.processKeyMovement(LEFT, deltaTime);
    if (glfwGetKey(glfwViewport, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam.processKeyMovement(RIGHT, deltaTime);
	if (glfwGetKey(glfwViewport, GLFW_KEY_F) == GLFW_PRESS)
		cam.toggleFreeCam();

	// Player movement (FIXME - move to server-side)
	if (glfwGetKey(glfwViewport, GLFW_KEY_W) == GLFW_PRESS)
		currentSpeed = RUN_SPEED;
	else if (glfwGetKey(glfwViewport, GLFW_KEY_S) == GLFW_PRESS)
		currentSpeed = -RUN_SPEED;
	else
		currentSpeed = 0;
	
	if (glfwGetKey(glfwViewport, GLFW_KEY_A) == GLFW_PRESS)
		currentTurnSpeed = -TURN_SPEED;
	else if (glfwGetKey(glfwViewport, GLFW_KEY_D) == GLFW_PRESS)
		currentTurnSpeed = TURN_SPEED;
	else
		currentTurnSpeed = 0;
	
}

// A callback function to resize the rendering window when the window is resized
void framebuffer_size_callback(GLFWwindow* glfwViewport, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* glfwViewport, double xpos, double ypos) {

	// Calculate offset based on previous position of the mouse
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	cam.processMouseMovement(xoffset, yoffset);
}

// Zoom callback for when mouse wheel is scrolled
void scroll_callback(GLFWwindow* glfwViewport, double xoffset, double yoffset)
{
	cam.processMouseScroll(yoffset);
}

//// int main() {
//// 	return render();
//// }