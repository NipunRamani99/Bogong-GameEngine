#include "../include/Game.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include "../include/Surface.hpp"
#include <GLFW/glfw3.h>
#define TO_DEG(x) 3.141592f*x/180.0f

struct KeyFrame {
	glm::vec3 Pos;
};
float lerp(float v0,float v1,float t) {
	return (1 - t)*v0 + t * v1;
}
glm::vec3 lerp(glm::vec3 v0, glm::vec3 v1, float t) {
	glm::vec3 retn_val;
	retn_val.x = lerp(v0.x, v1.x, t);
	retn_val.y = lerp(v0.y, v1.y, t);
	retn_val.z = lerp(v0.z, v1.z, t);
	return retn_val;
}
struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};
struct Light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
glm::vec3 light_colour = glm::vec3(1.0f, 1.0f, 1.0f);
Material material;
Light light;
glm::vec3 start_val = glm::vec3(4.0f,4.0f,0.0f);
glm::vec3 end_val = glm::vec3(-4.0f,4.0f,0.0f);
static float amt = 0.0f;
bogong::Game::Game()
{
	m_Shader.LoadShader("Shaders/ColourFragmentShader.glsl", ShaderType::FRAGMENT);
	m_Shader.LoadShader("Shaders/ColourVertexShader.glsl", ShaderType::VERTEX);
	m_Shader.LoadProgram();
	phong_shader.LoadShader("shaders/LightMapVertexShader.glsl", ShaderType::VERTEX);
	phong_shader.LoadShader("shaders/LightMapFragmentShader.glsl", ShaderType::FRAGMENT);
	phong_shader.LoadProgram();
	phong_shader.Bind();
	phong_shader.setVec3("light_pos", light_pos);
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	phong_shader.setVec3("light.ambient", light.ambient);
	phong_shader.setVec3("light.diffuse", light.diffuse);
	phong_shader.setVec3("light.specular", light.specular);
	material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.shininess = 2.0f;
	phong_shader.setVec3("material.ambient", material.ambient);
	phong_shader.setVec3("material.diffuse", material.diffuse);
	phong_shader.setVec3("material.specular", material.specular);
	phong_shader.setFloat("material.shininess", material.shininess);
	phong_shader.setVec3("light_colour", light_colour);
	cube = std::make_shared<Cube>(object_colour);
	cube->SetShader(phong_shader);
	light_cube = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), light_pos, glm::vec3(0.2f,0.2f,0.2f));
	light_cube->SetShader(m_Shader);
	camera = std::make_shared<FPCamera>();

}

void bogong::Game::Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse> &mouse,
	float delta,GLFWwindow * window)
{

	currentTime += delta;
	amt = 0.5 - 0.5*sinf(currentTime);
	glm::vec3 val = lerp(start_val, end_val, amt);
	light_pos = val;
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
	m_Shader.Bind();
	m_Shader.setMat4("projection", camera->GetProjection());
	m_Shader.setMat4("view", camera->GetView());
	phong_shader.Bind();
	phong_shader.setMat4("projection", camera->GetProjection());
	phong_shader.setMat4("view", camera->GetView());
	ImGui::Begin("Bogong");
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400.0f, 500.0f));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Cube 1:");
	cube->Update("1");
	ImGui::InputFloat("Total Time: ", (float*)&totalTime, 4);
	ImGui::InputFloat3("Start Val: ", (float*)&start_val, 4);
	ImGui::InputFloat3("End Val: ", (float*)&end_val, 4);
	ImGui::InputFloat("Lerp amt: ", &amt, 4);
	phong_shader.setVec3("light_pos", light_pos);
	light_cube->Translate(light_pos);

	if (ImGui::InputFloat3("Light Pos", (float*)&light_pos, 4)) {
		light_cube->Translate(light_pos);
		phong_shader.setVec3("light_pos", light_pos);
	}
	phong_shader.setVec3("viewPos", camera->GetPos());

	if (ImGui::Button("Camera Debug")) { camera->ToggleDebug(); }
	ImGui::End();
	ImGui::Begin("Material and Light");
	if (ImGui::InputFloat3("Material Ambient", (float*)&material.ambient, 4))
	{
		phong_shader.setVec3("material.ambient", material.ambient);
	}
	if (ImGui::InputFloat3("Material diffuse", (float*)&material.diffuse, 4))
	{
		phong_shader.setVec3("material.diffuse", material.diffuse);
	}
	if (ImGui::InputFloat3("Material Specular", (float*)&material.specular, 4))
	{
		phong_shader.setVec3("material.specular", material.specular);
	}
	if (ImGui::InputFloat("Material Shininess", (float*)&material.shininess, 4))
	{
		phong_shader.setFloat ("material.shininess", material.shininess);
	}

	if (ImGui::InputFloat3("Light Ambient", (float*)&light.ambient, 4))
	{
		phong_shader.setVec3("light.ambient", light.ambient);
	}
	if (ImGui::InputFloat3("Light Diffuse", (float*)&light.diffuse, 4))
	{
		phong_shader.setVec3("light.ambient", light.diffuse);
	}
	if (ImGui::InputFloat3("Light Specular", (float*)&light.specular, 4))
	{
		phong_shader.setVec3("light.ambient", light.specular);
	}
	ImGui::End();
}

void bogong::Game::Draw() const
{

	light_cube->Draw();
	cube->Draw();
	assert(!error());
}