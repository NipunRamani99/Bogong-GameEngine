#include "ImGuiPane.hpp"
#include "Imgui.h"
#include "Timer.hpp"
bogong::ImGuiPane::ImGuiPane(std::string pane_name, glm::vec2 pos, glm::vec2 size)
	:
	name(pane_name),
	pos(pos.x,pos.y),
	size(size.x,size.y),
	array_index(0)
{
	
	for (int i = 0; i < 300; i++) {
		frame_times[i] = float(i) * 0.2f;
	}
}

void  bogong::ImGuiPane::Render() {
	//ImGui::Begin(name.c_str());
	//ImGui::PlotLines("", &frame_times[0], frame_times.size(), array_index, (const char*)0, 0.0f, max_frame_time + 1.0f, ImVec2(255, 70));
	//
	//ImGui::Text("Frametime: %.3f (FPS: %.1f) ", frame_time, ImGui::GetIO().Framerate);
	//std::string str = "Max Frame time: " + std::to_string(max_frame_time);
	//ImGui::Text(str.c_str());
	//
	//for (auto & p : Timer::durations) {
	//	std::string str = p.first + ": %.4f";
	//	auto val = double(p.second.count())/ 1000000.0f;
	//	ImGui::Text(str.c_str(),val);
	//}
	//ImGui::End();
}

void bogong::ImGuiPane::UpdatePane() {
	frame_time = 1000.0f / ImGui::GetIO().Framerate;
	if (frame_time > max_frame_time) {
		max_frame_time = frame_time;
	}
	array_index = (array_index + 1) % frame_times.size();
	frame_times[array_index] = frame_time;

}
