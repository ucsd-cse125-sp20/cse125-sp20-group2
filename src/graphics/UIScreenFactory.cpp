#include <graphics/UIScreenFactory.h>

UIScreenFactory::UIScreenFactory() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(150.0f, 100.0f));
}

UIScreenFactory::~UIScreenFactory() {

}

void UIScreenFactory::UIInventory(std::unordered_map<int, Ingredient*>* map) {
	corner = 0;
	window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2((float)100, (float)(40 * map->size())));
	ImGui::Begin("Inventory");
	std::unordered_map<int, Ingredient*>::iterator it = map->begin();
	while (it != map->end())
	{
		UIText(it->second->getDetailedName());
		it++;
	}
	UIEnd();
}

void UIScreenFactory::UIGameInfo(int round, int32_t minutes, int32_t seconds) {
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
	window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2((float)100, 30+ (float)(40 * map->size())));
	ImGui::Begin("Inventory");
	std::unordered_map<int, Ingredient*>::iterator it = map->begin();
	while (it != map->end())
	{
		if (ImGui::Button(it->second->getDetailedName().c_str())) {
			ret = it->second;
			std::cout<<"Ingredient clicked"<<std::endl;
		}
		it++;
		
	}
	UIEnd();
	return ret;
}

void UIScreenFactory::UICookingEvent(std::string msg) {
	window_pos = ImVec2(io.DisplaySize.x / 2 - DISTANCE, 2*DISTANCE);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(50.0f, 40.0f));
	if (ImGui::Begin(" "))
	{
		UIText(msg);
	}
	UIEnd();
}

void UIScreenFactory::UIScore(int score) {
	window_pos = ImVec2(io.DisplaySize.x / 2 - DISTANCE, DISTANCE);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	setWindowSize(ImVec2(60.0f, 40.0f));
	if (ImGui::Begin("Score"))
	{
		std::string scoreStr = std::to_string(score);
		UIText(scoreStr);
	}
	UIEnd();
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