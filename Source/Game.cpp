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
	plane = std::make_shared<Plane>();
	plane->SetShader(m_Shader);
	camera = std::make_shared<FPCamera>();
}

void bogong::Game::Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse> &mouse,
	float delta)
{
	m_Shader.Bind();
	camera->Update(kbd, mouse,delta);
	m_Shader.setMat4("projection", camera->GetProjection());
	m_Shader.setMat4("view", camera->GetView());
}

void bogong::Game::Draw() const
{
	assert(!error(), __LINE__);
	plane->Draw();
	assert(!error());
}
