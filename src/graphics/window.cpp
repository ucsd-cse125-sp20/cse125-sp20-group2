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

void Window::addInventory(std::unordered_map<int, Ingredient*>* inventoryPtr) {
	this->inventory = inventoryPtr;
}

void Window::toggleCursor()
{
	// Disable cursor
	if (glfwGetInputMode(glfwViewport, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
	{
		glfwSetInputMode(glfwViewport, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// Enable cursor
	else
	{
		glfwSetInputMode(glfwViewport, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
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
	/*glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);	*/
	GLFWwindow* glfwViewport = glfwCreateWindow((int)Config::getFloat("Window_Width"), (int)Config::getFloat("Window_Height"), Config::get("Window_Title").c_str(), NULL, NULL);
	
	// Mouse centering
	glfwSetCursorPos(glfwViewport, lastX, lastY);

	if (glfwViewport == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Current context is window
	glfwMakeContextCurrent(glfwViewport);

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
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(glfwViewport, true);

	ImGui_ImplOpenGL3_Init("#version 130");
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("assets/fonts/NikkyouSans-B6aV.ttf", 14);
	ImGui::PushFont(font1);

}

void Window::addCookingEventMessage(std::string msg) {
	this->cookingEventMsg = msg;
}

void Window::removeCookingEventMessage() {
	this->cookingEventMsg = "";
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

int Window::getRound() {
	return this->round;
}

Ingredient* Window::getSelectedIngredient() {
	return this->selectedIngredient;
}

void Window::updateRound(Game::RoundInfo_RoundState roundState) {
	switch (roundState) {
        case Game::RoundInfo_RoundState_LOBBY: {
            this->setRound(0);
            break;
        }
        case Game::RoundInfo_RoundState_DUNGEON: {
            this->setRound(1);
            break;
        } 
        case Game::RoundInfo_RoundState_DUNGEON_WAITING: {
            this->setRound(2);
            break;
        }
        case Game::RoundInfo_RoundState_KITCHEN:
        {
            this->setRound(3);
            break;
        }
        case Game::RoundInfo_RoundState_KITCHEN_WAITING: {
            this->setRound(4);
			break;
        }
    } 
}

void Window::setScore(int score) {
	this->score = score;
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

	// camera projection transformation
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(this->camera->fov), Config::getFloat("Window_Width") / Config::getFloat("Window_Height"), 0.1f, 100.0f);

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

	UIScreenFactory ui = UIScreenFactory();
	ui.UIGameInfo(this->round, minutes, seconds);
	ui.UIScore(this->score);
	
	Ingredient* tmp;

	if( this->inventory != NULL )
		if( this->round == DUNGEON )
			ui.UIInventory(this->inventory);
		else
			tmp = ui.UIButtonInventory(this->inventory);
	selectedIngredient = tmp != NULL? tmp: selectedIngredient;

	if( this->cookingEventMsg.compare("") != 0 )
		ui.UICookingEvent(this->cookingEventMsg);

	if (gameOver)
	{
		ui.UIGameOver(gameWin);
	}
	

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