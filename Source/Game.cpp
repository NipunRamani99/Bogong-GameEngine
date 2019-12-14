#include "../include/Game.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include "../include/Surface.hpp"
bogong::Game::Game()
{
	m_Shader.LoadShader("Shaders/SimpleFragmentShader.glsl", ShaderType::FRAGMENT);
	m_Shader.LoadShader("Shaders/SimpleVertexShader.glsl", ShaderType::VERTEX);
	m_Shader.LoadProgram();
	assert(!error());
	quad = std::make_shared<Quad>();
	quad->SetShader(m_Shader);
	assert(!error());
	
}

void bogong::Game::Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse> &mouse,
	float delta)
{
	
}

void bogong::Game::Draw() const
{
	quad->Draw();
	assert(!error());
}
