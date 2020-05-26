#include <gui/imgui_impl_glfw.h>
#include <gui/imgui_impl_opengl3.h>
#include <string>
#include <unordered_map>

class UIScreenFactory {
public:
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos;
	ImVec2 window_pos_pivot;

	UIScreenFactory();
	~UIScreenFactory();

	void UIGameInfo(int round, int32_t minutes, int32_t seconds);
	void UIScore(int score);
	void UIText(const char* str);
	void UIEnd();
	void setWindowSize(ImVec2 size);

private:
	int corner = 1;
	const float DISTANCE = 10.0f;
};