#pragma once
#include <string>
#include <glm/glm.hpp>
#include <array>
#include "Imgui.h"
namespace bogong {
	class ImGuiPane {
	public:
		ImGuiPane(std::string pane_name, glm::vec2 pos, glm::vec2 size);
		
		void Render();
		void UpdatePane();
	private:
		std::string name;
		ImVec2 pos;
		ImVec2 size;
		float frame_time = 0.0f;
		float max_frame_time = 0.0f;

		std::array<float, 300> frame_times;
		int array_index;

	};
}