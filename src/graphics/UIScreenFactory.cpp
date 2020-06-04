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
	this->dungeon_waiting_width = 0;
	this->dungeon_waiting_height = 0;
	this->dungeon_waiting_texture = 0;
	bool ret = LoadTextureFromFile("assets/images/DungeonWaitingScreen.png", &dungeon_waiting_texture, &dungeon_waiting_width, &dungeon_waiting_height);
	IM_ASSERT(ret);

	this->kitchen_waiting_width = 0;
	this->kitchen_waiting_height = 0;
	this->kitchen_waiting_texture = 0;
	ret = LoadTextureFromFile("assets/images/KitchenWaitingScreen.png", &kitchen_waiting_texture, &kitchen_waiting_width, &kitchen_waiting_height);
	IM_ASSERT(ret);

	this->lobby_width = 0;
	this->lobby_height = 0;
	this->lobby_texture = 0;
	ret = LoadTextureFromFile("assets/images/LobbyScreen.png", &lobby_texture, &lobby_width, &lobby_height);
	IM_ASSERT(ret);
}

void UIScreenFactory::UIInventory(std::unordered_map<int, Ingredient*>* map) {
	corner = 0;
	window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	// setWindowSize(ImVec2((float)150, (float)(250)));
	ImGui::Begin("Inventory");
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	std::unordered_map<int, Ingredient*>::iterator it = map->begin();
	ImGui::BeginChild("Scrolling");
	while (it != map->end())
	{
		UIText(it->second->getDetailedName());
		it++;
	}
	ImGui::EndChild();
	ImGui::End();
}

void UIScreenFactory::UIGameInfo(int round, int32_t minutes, int32_t seconds) {
	corner = 1;
	window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(Config::getFloat("Game_Info_Window_Width"), Config::getFloat("Game_Info_Window_Height")));
	if (ImGui::Begin("Game Info"))
	{
		ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
		std::string roundInfo;
		switch(round) {
			case LOBBY_NUM: 
				UIText(LOBBY_STR);
				ImGui::End();
				return;
			case DUNGEON_NUM: roundInfo = DUNGEON_STR; break;
			case KITCHEN_NUM: roundInfo = kitchen; break;
			default: roundInfo = "TRANSITION";
		}
		UIText(roundInfo);
		std::string str = "Time Left: ";
		str = str.append(std::to_string(minutes));
		str = str.append(" : ");
		str = str.append(std::to_string(seconds));
		UIText(str);
	}
	ImGui::End();
}

void UIScreenFactory::UIGameOver(bool gameWin) {
	ImGui::SetNextWindowSize(ImVec2(io->DisplaySize.x-DISTANCE, io->DisplaySize.y-DISTANCE));
	ImGui::Begin("TIME IS UP");
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	if (gameWin) ImGui::Text("You win! You are chosen as the next face of the burdgeoning communist fast-food industry");
	else ImGui::Text("A comrade outperformed you and slackers like you in the glorious communist state are an enemy of the people. You were sent to a gulag to live out your days doing hard labor. Maybe you could still fulfill the prison labor quota.");
	ImGui::End();
}

Ingredient* UIScreenFactory::UIButtonInventory(std::unordered_map<int, Ingredient*>* map) {
	Ingredient* ret = NULL;
	corner = 0;
	window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	// setWindowSize(ImVec2((float)250, (float)250));
	ImGui::Begin("Inventory");
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	std::unordered_map<int, Ingredient*>::iterator it = map->begin();
	ImGui::BeginChild("Scrolling");
	while (it != map->end())
	{
		if (this->highlighted == it->second) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
			if( ImGui::Button(it->second->getDetailedName().c_str()) ) {
				ret = it->second;
				std::cout<<"Ingredient clicked"<<std::endl;
				this->highlighted = it->second;
			}
			ImGui::PopStyleColor();
		}
		else if (ImGui::Button(it->second->getDetailedName().c_str())) {
			ret = it->second;
			std::cout<<"Ingredient clicked"<<std::endl;
			this->highlighted = it->second;
		}
		it++;
		
	}
	ImGui::EndChild();
	ImGui::End();
	return ret;
}

void UIScreenFactory::UICookingEvent(std::string msg) {
	/**
	corner = 3;
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	window_pos = ImVec2(io->DisplaySize.x / 2 - DISTANCE, 2*DISTANCE);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(Config::getInt("Cooking_Event_"), 40.0f));
	if (ImGui::Begin(" "))
	{
		ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
		UIText(msg);
	}
	ImGui::End();
	**/
}

void UIScreenFactory::UIScore(int score) {
	corner = 0;
	window_pos = ImVec2(io->DisplaySize.x / 2 - (DISTANCE/2), DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(Config::getFloat("Score_Window_Width"), Config::getFloat("Score_Window_Height")));
	if (ImGui::Begin("Score"))
	{
		ImGui::SetWindowFontScale(Config::getFloat("Score_Font_Scale"));
		std::string scoreStr = std::to_string(score);
		UIText(scoreStr);
	}
	ImGui::End();
}

void UIScreenFactory::UIDungeonInstructions() {
	if( dungeonFirst ) {
		ImGui::SetNextWindowCollapsed(false, 0);
		dungeonFirst = false;
	}
	ImGui::Begin("Dungeon Instructions");
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	ImGui::Image((void*)(intptr_t)dungeon_waiting_texture, ImVec2(dungeon_waiting_width, dungeon_waiting_height));
	ImGui::End();
}

void UIScreenFactory::UIInstructionSet(std::vector<std::string> instructions, std::string recipeName ) {
	std::string title = "Recipe" + recipeName;
	ImGui::Begin(title.c_str());
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	ImGui::BeginChild("Scrolling");
	for( auto str: instructions ) {
		UIText(str);
	}
	ImGui::EndChild();
	ImGui::End();
}

void UIScreenFactory::UIKitchenInstructions() {
	if( kitchenFirst ) {
		ImGui::SetNextWindowCollapsed(false, 0);
		kitchenFirst = false;
	}
	ImGui::Begin("Dungeon Instructions");
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	ImGui::Image((void*)(intptr_t)kitchen_waiting_texture, ImVec2(kitchen_waiting_width, kitchen_waiting_height));
	ImGui::End();
}

void UIScreenFactory::UILobbyScreen() {
	if( lobbyFirst ) {
		ImGui::SetNextWindowCollapsed(false, 0);
		lobbyFirst = false;
	}
	ImGui::Begin("Lobby Screen");
	ImGui::SetWindowFontScale(Config::getFloat("Font_Scale"));
	ImGui::Image((void*)(intptr_t)lobby_texture, ImVec2(lobby_width, lobby_height));
	ImGui::End();
}

void UIScreenFactory::UIText(std::string str) {
	ImGui::Text(str.c_str());
}

void UIScreenFactory::setWindowSize(ImVec2 size) {
	ImGui::SetNextWindowSize(size);
}
