#include "window.h"

// Initial mouse positions
float lastX, lastY;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* glfwViewport, int width, int height);
void processInput(GLFWwindow* glfwViewport);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window::Window(int width = Config::getFloat("Window_Width"), int height = Config::getFloat("Window_Height")) : objNum (0) {
	this->isClosed = false;
	this->width = width;
	this->height = height;
	this->setupWindow();
	this->shader = new Shader(Config::get("Vertex_Shader"), Config::get("Fragment_Shader"));
	this->UIshader = new Shader("src//graphics//shaders//ui_vert_shader.glsl", "src//graphics//shaders//ui_frag_shader.glsl"); 
	///TODO: Need to add to Config?
	this->camera = new Camera(Config::getVec3("Camera_Location"));
	this->inventory = NULL;
}

void Window::addObject(unsigned int id, GameObject* object) {
	this->objectsToRender.insert({id, object});
	objNum++;
}

void Window::removeObject(unsigned int index) {
	this->objectsToRender.erase(index);
	objNum--;
}

void Window::addInventory(std::unordered_map<int, IngredientObject*>* inventoryPtr) {
	this->inventory = inventoryPtr;
}

void Window::setupWindow() {

	// inital mouse positions
	lastX = Config::getFloat("Window_Width") / 2, lastY = Config::getFloat("Window_Height") / 2;

	// // //
	// Setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		
	GLFWwindow* glfwViewport = glfwCreateWindow((int)Config::getFloat("Window_Width"), (int)Config::getFloat("Window_Height"), Config::get("Window_Title").c_str(), NULL, NULL);

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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(glfwViewport, true);

	ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::close() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	if (glfwWindowShouldClose(glfwViewport))
	{
		isClosed = true; 
		glfwTerminate();
	}
	else std::cerr << "ERROR: Unable to close window!" << std::endl;
}

void Window::setTimer(int64_t timer) {
	this->timer = timer;
}

void Window::setRound(int round) {
	this->round = round;
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
	shader->setVec3("viewPos", this->camera->pos);
	
	// Target following
    if (!camera->freeCam) camera->warpToTarget();

	// // //
	// View + perspective projection matrices

	// camera view transformation (related to xy axis rotations)
	glm::mat4 view = this->camera->getViewMatrix();

	// camera projection transformation (related to zooms)
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(this->camera->zoom), Config::getFloat("Window_Width") / Config::getFloat("Window_Height"), 0.1f, 100.0f);

	// pass transformation matrices to the shader
	shader->setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader->setMat4("view", view);

	// Update camera view appropriately
	cameraViewUpdate();

	// // //
	// Render each GameObject

	for (auto it = objectsToRender.begin(); it != objectsToRender.end(); ++it) {
		GameObject* obj = it->second;

		if (!obj->getRender()) continue;

		// load model if needed
		if (!obj->model) obj->loadModel();

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
	int32_t  minutes = this->timer / 60;
	int32_t seconds = this->timer % 60;
	int corner = 1;
	const float DISTANCE = 10.0f;
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(150.0f, 100.0f));
    if (ImGui::Begin("Game Info"))
    {
		std::string roundInfo = "Round ";
		roundInfo = roundInfo.append(std::to_string(this->round));
		ImGui::Text(roundInfo.c_str());
		std::string str = "Time Left: " + minutes;
		str = str.append(std::to_string(minutes));
		str = str.append(" : ");
		str = str.append(std::to_string(seconds));
        ImGui::Text(str.c_str());
    }
    ImGui::End();
	ImGui::SetNextWindowSize(ImVec2((float)100, (float)(40*this->inventory->size())));
	ImGui::Begin("Inventory");                         
	if (this->inventory != NULL) {
		std::unordered_map<int, IngredientObject*>::iterator it = this->inventory->begin();
		while (it != this->inventory->end())
		{
			ImGui::Text(it->second->getName().c_str());
			it++;
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void Window::cameraViewUpdate() {
	double xpos, ypos;
	glfwGetCursorPos(glfwViewport, &xpos, &ypos);

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera->processMouseMovement(xoffset, yoffset);
}