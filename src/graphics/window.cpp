#include "window.h"

//// Camera at desiginated position
Camera cam(INITIAL_CAM_POS);

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
		std::cerr << "Failed to create GLFW window" << std::endl;
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
		std::cerr << "Failed to load GLAD" << std::endl;
	}

	// Enable depth testing
	// This is used to make sure we occlude textures when a
	// surface is obscured by another surface (i.e. in 3D objects).
	glEnable(GL_DEPTH_TEST);

	// Set window var
	this->glfwViewport = glfwViewport;
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
	// per-frame time logic
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

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
		GameObject* obj = it->second;

		// Set respective model matrix for each object and send it to the shader.
		glm::mat4 mat = glm::mat4(1.0);
		mat = glm::translate(mat, obj->getPosition());
		mat = glm::scale(mat, obj->getScaleVec());
		mat = glm::rotate(mat, obj->getRotation(), UP);
		shader->setMat4("model", mat);

		// Used to convert normal vectors to world space coordinates, without applying translations to them
		shader->setMat4("normalMatrix", glm::transpose(glm::inverse(mat)));

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