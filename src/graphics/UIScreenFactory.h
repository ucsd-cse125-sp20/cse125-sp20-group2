#include <gui/imgui_impl_glfw.h>
#include <gui/imgui_impl_opengl3.h>
#include <string>
#include <unordered_map>
#include <objects/Ingredient.h>
#include <constants/gameplay_vars.h>

class UIScreenFactory {
public:
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos;
	ImVec2 window_pos_pivot;

	UIScreenFactory();
	~UIScreenFactory();

	void UIGameInfo(int round, int32_t minutes, int32_t seconds);
	void UIScore(int score);
	void UIInventory(std::unordered_map<int, Ingredient*>* map);
	Ingredient* UIButtonInventory(std::unordered_map<int, Ingredient*>* map);
	void UIGameOver(bool gameWin);

	void UIText(std::string str);
	void UIEnd();
	void setWindowSize(ImVec2 size);

private:

	const std::string lobby = "LOBBY";
	const std::string dungeon = "DUNGEON";
	const std::string kitchen = "KITCHEN";
	int corner = 1;
	const float DISTANCE = 10.0f;
};