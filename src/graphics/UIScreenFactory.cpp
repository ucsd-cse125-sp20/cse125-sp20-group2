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

void UIScreenFactory::UIGameInfo(int round, int32_t minutes, int32_t seconds) {
	if (ImGui::Begin("Game Info"))
	{
		std::string roundInfo = "Round ";
		roundInfo = roundInfo.append(std::to_string(round));
		UIText(roundInfo.c_str());
		std::string str = "Time Left: " + minutes;
		str = str.append(std::to_string(minutes));
		str = str.append(" : ");
		str = str.append(std::to_string(seconds));
		UIText(str.c_str());
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
		UIText(scoreStr.c_str());
	}
	UIEnd();
}

void UIScreenFactory::UIText(const char* str) {
	ImGui::Text(str);
}

void UIScreenFactory::UIEnd() {
	ImGui::End();
}

void UIScreenFactory::setWindowSize(ImVec2 size) {
	ImGui::SetNextWindowSize(size);
}