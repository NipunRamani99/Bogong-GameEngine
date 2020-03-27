#pragma once
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
namespace bogong {
	namespace Init {
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.00f, 1.00f);

		void InitImgui(GLFWwindow & window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();
			const char* glsl_version = "#version 330";
			// Setup Platform/Rendeer bindings
			ImGui_ImplGlfw_InitForOpenGL(&window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		void StartImguiFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void Render()
		{
			ImGui::Render();
		}
		void EndImguiFrame()
		{

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		}
	}
}