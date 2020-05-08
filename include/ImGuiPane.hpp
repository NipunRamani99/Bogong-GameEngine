#pragma once
#include "Imgui.h"
#include <string>
#include <glm/glm.hpp>
#include <array>
namespace bogong {
	class ImGuiPane {
	public:
		ImGuiPane(std::string pane_name, glm::vec2 pos,glm::vec2 size)
			:
			name(pane_name),
			pos(pos)
		{

		}
		void Render() {

		}
		void UpdatePane() {

		}
	private:
		std::string name;
		glm::vec2 pos;
		glm::vec2 size;
		float frame_time = 0.0f;
		std::array<float, 300> frame_times;
		int array_index;

	};
}