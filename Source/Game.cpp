#include "../include/Game.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include "../include/Surface.hpp"
#include <GLFW/glfw3.h>
#define TO_DEG(x) 3.141592f*x/180.0f
/*  //Important shit
	if (kbd->isKeyPressed(KEY::KEY_K))
	{
		if (canToggle) {
			isMouseEnabled = !isMouseEnabled;
			canToggle = false;
			if (!isMouseEnabled) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(window, 400, 300);
				mouse->x = 400;
				mouse->y = 300;
				camera->ClearMouse(mouse);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}

	}
	if (!isMouseEnabled)
		camera->Update(kbd, mouse, delta);
	if (!canToggle) {
		timer += delta;
		if (timer >= 0.1f) {
			canToggle = true;
			timer = 0.0f;
		}
	}

*/

bogong::Game::Game()
{
	scene = std::make_shared<SampleScene>();
	scr = std::make_shared<Screen>();
	camera = std::make_shared<FPCamera>();
}

void bogong::Game::Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse> &mouse,
	float delta,GLFWwindow * window)
{
	camera->Update(kbd, mouse, delta);
	
	scr->Update(camera->GetPos(), camera->GetDir(), camera->GetView());
	scene->Update(kbd, mouse, delta);
	ImGui::Begin("Test");
	ImGui::End();
}

void bogong::Game::Draw(float delta) const
{
	scr->Draw(delta);
	glClear(GL_DEPTH_BUFFER_BIT);
	scene->Draw();
	
	
	
}