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
glm::vec3 start_val = glm::vec3(0.0f);
glm::vec3 end_val = glm::vec3(1.0f);
bogong::Game::Game()
{
	m_Shader.LoadShader("Shaders/SimpleFragmentShader.glsl", ShaderType::FRAGMENT);
	m_Shader.LoadShader("Shaders/SimpleVertexShader.glsl", ShaderType::VERTEX);
	m_Shader.LoadProgram();
	phong_shader.LoadShader("shaders/PhongVertexShader.glsl", ShaderType::VERTEX);
	phong_shader.LoadShader("shaders/PhongFragmentShader.glsl", ShaderType::FRAGMENT);
	phong_shader.LoadProgram();
	phong_shader.Bind();
	phong_shader.setVec3("light_pos", light_pos);	
	phong_shader.setFloat("ambient_str", 0.2f);
	assert(!error());
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
	float amt = sinf( currentTime );
	glm::vec3 val = lerp(start_val, end_val, amt);
	


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
	ImGui::Text("Cube 1:");
	cube->Update("1");
	light_pos = val;
	ImGui::InputFloat("Total Time: ", (float*)&totalTime, 4);
	ImGui::InputFloat3("Start Val: ", (float*)&start_val, 4);
	ImGui::InputFloat3("End Val: ", (float*)&end_val, 4);

	phong_shader.setVec3("light_pos", light_pos);
	light_cube->Translate(light_pos);

	if (ImGui::InputFloat3("Light Pos", (float*)&light_pos, 4)) {
		light_cube->Translate(light_pos);
		phong_shader.setVec3("light_pos", light_pos);
	}


}

void bogong::Game::Draw() const
{
	assert(!error(), __LINE__);
	cube->Draw();
	light_cube->Draw();
	assert(!error());
}
