#include <graphics/UIScreenFactory.h>

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

UIScreenFactory::UIScreenFactory() {
	
}

UIScreenFactory::~UIScreenFactory() {

}

void UIScreenFactory::setUpFrame() {
	io = &ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UIScreenFactory::loadImages() {
	this->shopping_list_width = 0;
	this->shopping_list_height = 0;
	this->shopping_list_texture = 0;
	bool ret = LoadTextureFromFile("assets/images/ShoppingList.png", &shopping_list_texture, &shopping_list_width, &shopping_list_height);
	IM_ASSERT(ret);
}

void UIScreenFactory::UIInventory(std::unordered_map<int, Ingredient*>* map) {
	corner = 0;
	window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2((float)150, (float)(250)));
	ImGui::Begin("Inventory");
	std::unordered_map<int, Ingredient*>::iterator it = map->begin();
	ImGui::BeginChild("Scrolling");
	while (it != map->end())
	{
		UIText(it->second->getDetailedName());
		it++;
	}
	ImGui::EndChild();
	UIEnd();
}

void UIScreenFactory::UIGameInfo(int round, int32_t minutes, int32_t seconds) {
	corner = 1;
	window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(150.0f, 100.0f));
	if (ImGui::Begin("Game Info"))
	{
		std::string roundInfo;
		switch(round) {
			case LOBBY: roundInfo = lobby; break;
			case DUNGEON: roundInfo = dungeon; break;
			case KITCHEN: roundInfo = kitchen; break;
			default: roundInfo = "TRANSITION";
		}
		UIText(roundInfo);
		std::string str = "Time Left: " + std::to_string(minutes);
		str = str.append(std::to_string(minutes));
		str = str.append(" : ");
		str = str.append(std::to_string(seconds));
		UIText(str);
	}
	UIEnd();
}

void UIScreenFactory::UIGameOver(bool gameWin) {
	ImGui::SetNextWindowSize(ImVec2(200.0f, 200.0f));
	ImGui::Begin("Game Over!");
	if (gameWin) ImGui::Text("You win!");
	else ImGui::Text("You lose!");
	ImGui::End();
}

Ingredient* UIScreenFactory::UIButtonInventory(std::unordered_map<int, Ingredient*>* map) {
	Ingredient* ret = NULL;
	corner = 0;
	window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2((float)150, (float)250));
	ImGui::Begin("Inventory");
	std::unordered_map<int, Ingredient*>::iterator it = map->begin();
	ImGui::BeginChild("Scrolling");
	while (it != map->end())
	{
		if (it->second->getClicked()) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
			ImGui::Button(it->second->getDetailedName().c_str());
			ImGui::PopStyleColor();
		}
		else if (ImGui::Button(it->second->getDetailedName().c_str())) {
			ret = it->second;
			std::cout<<"Ingredient clicked"<<std::endl;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
			ImGui::PopStyleColor();
			it->second->setClicked(true);
		}
		it++;
		
	}
	ImGui::EndChild();
	UIEnd();
	return ret;
}

void UIScreenFactory::UICookingEvent(std::string msg) {
	corner = 3;
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	window_pos = ImVec2(io->DisplaySize.x / 2 - DISTANCE, 2*DISTANCE);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(80.0f, 40.0f));
	if (ImGui::Begin(" "))
	{
		UIText(msg);
	}
	UIEnd();
}

void UIScreenFactory::UIScore(int score) {
	window_pos = ImVec2(io->DisplaySize.x / 2 - (DISTANCE/2), DISTANCE);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(70.0f, 80.0f));
	if (ImGui::Begin("Score"))
	{
		std::string scoreStr = std::to_string(score);
		UIText(scoreStr);
	}
	UIEnd();
}

void UIScreenFactory::UIShoppingList() {
	ImGui::Begin("Your Assigned Shopping List");
	ImGui::Image((void*)(intptr_t)shopping_list_texture, ImVec2(shopping_list_width, shopping_list_height));
	ImGui::End();
}

void UIScreenFactory::UIText(std::string str) {
	ImGui::Text(str.c_str());
}

void UIScreenFactory::UIEnd() {
	ImGui::End();
}

void UIScreenFactory::setWindowSize(ImVec2 size) {
	ImGui::SetNextWindowSize(size);
}