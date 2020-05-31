#include "../include/Game.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include "../include/Surface.hpp"
#include <GLFW/glfw3.h>
#define TO_DEG(x) 3.141592f*x/180.0f


bogong::Game::Game()
{
	scene = std::make_shared<SampleScene>();
	scr = std::make_shared<Screen>();
	camera = std::make_shared<FPCamera>();
}

void bogong::Game::Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse> &mouse,
	float delta,GLFWwindow * window)
{
	scene->Update(kbd, mouse, delta,window);
}

void bogong::Game::Draw(float delta) const
{
	scene->Draw();
}