#pragma once
#include"Nodes/NodeBase.hpp"
#include "../Camera.h"
#include <stack>
#include "../Keyboard.h"
#include "../Mouse.h"
#include "../Imgui.h"
namespace bogong {
	class Scene {

		std::shared_ptr<node::NodeBase> root_node;
		std::shared_ptr<FPCamera> cam;
		float timer = 0.0f;
		bool canToggle = true;
		bool isMouseEnabled = false;
	public:
		Scene();
		void SetRootNode(std::shared_ptr<node::NodeBase> node);
		void SetCamera(std::shared_ptr<FPCamera> cam) {
			this->cam = cam;
		}
		std::shared_ptr<FPCamera> getCamera() {
			return cam;
		}
		glm::mat4 getProjection() {
			return cam->GetProjection();
		}
		glm::mat4 getView()
		{
			return cam->GetView();
		}
		std::shared_ptr<node::NodeBase> getRootNode() {
			return root_node;
		}
		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse,float delta,GLFWwindow * window){
			if (kbd->isKeyPressed(KEY::KEY_K))
			{
				if (canToggle) {
					isMouseEnabled = !isMouseEnabled;
					canToggle = false;
					if (!isMouseEnabled) {
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						glfwSetCursorPos(window, 640, 360);
						mouse->x = 640;
						mouse->y = 360;
						cam->ClearMouse(mouse);
						ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
					}
					else {
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						
						ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
					}
				}

			}
			if (!isMouseEnabled)
				cam->Update(kbd, mouse, delta);
			if (!canToggle) {
				timer += delta;
				if (timer >= 0.1f) {
					canToggle = true;
					timer = 0.0f;
				}
			}
			
		}
	};
}