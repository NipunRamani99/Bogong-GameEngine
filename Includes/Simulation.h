#pragma once
#include "Shaders.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ICallbacks.h"
#include "Globals.h"
#include "Grid/Grid.hpp"
#include "CameraPosVisualizer.hpp"
class Simulation
{
private:

	glm::vec3 lightPos = glm::vec3(-1.0f,0.2f,0.0f);
	Shader m_Shader;
	Shader m_GridShader;
	Shader camPosShader;
	std::shared_ptr<CameraPosVisualizer> visualizer;
	std::shared_ptr<Grid> m_Grid;
	float m_Scale = 0.895f;
public:
	Simulation() = default;
	Simulation(Shader p_Shader)
		:
		m_Grid(std::make_shared<Grid>(glm::vec3(1.0f,0.0f,0.0f)))
		{  
		m_Shader      = p_Shader;
		m_Scale       = 0.001f;
		ICallbacks::SetShader(m_Shader);
		camPosShader.LoadShader("Shaders/camPosShader.vert", ShaderType::VERTEX);
		camPosShader.LoadShader("Shaders/Grid.frag", ShaderType::FRAGMENT);
		camPosShader.LoadProgram();
		m_GridShader.LoadShader("Shaders/Grid.vert", ShaderType::VERTEX);
		m_GridShader.LoadShader("Shaders/Grid.frag", ShaderType::FRAGMENT);
		m_GridShader.LoadProgram();
		ICallbacks::AddShader(m_GridShader);
		error();
		ICallbacks::AddShader(camPosShader);
		error();
		visualizer = std::make_shared<CameraPosVisualizer>();
		visualizer->SetShader(camPosShader);
		m_Grid->SetShader(m_GridShader);
	}
	void Begin()
	{
		
	}
	void Update()
	{
		camPosShader.Bind();
		camPosShader.setVec3("cameraPos", CamGlobal::viewPos);
		error();
	}
	void Draw()
	{		
		visualizer->Draw();
		m_Grid->Draw();
		error();
	}
};   