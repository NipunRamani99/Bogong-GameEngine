#pragma once
#include "Shaders.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ICallbacks.h"
#include "Globals.h"
#include "Grid/Grid.hpp"
class Simulation
{
private:

	glm::vec3 lightPos = glm::vec3(-1.0f,0.2f,0.0f);
	Shader m_Shader;
	Shader m_GridShader;
	
	std::shared_ptr<Grid> grid;
	float m_Scale = 0.895f;
public:
	Simulation() = default;
	Simulation(Shader p_Shader)
		:
		grid(std::make_shared<Grid>(Grid(glm::vec3(0.27450f,0.27450f,0.27450f))))
		{  
		m_Shader      = p_Shader;
		m_Scale       = 0.001f;
		//error();
		ICallbacks::SetShader(m_Shader);
		m_GridShader.LoadShader("Shaders/Grid.vert", ShaderType::VERTEX);
		m_GridShader.LoadShader("Shaders/Grid.frag", ShaderType::FRAGMENT);
		m_GridShader.LoadProgram();
		ICallbacks::AddShader(m_GridShader);
		grid->SetShader(m_GridShader);
		//error();
	}
	//Simulation(Simulation && simulation)
	//{
	//	m_Shader     = std::move(simulation.m_Shader);
	//	m_Scale      = std::move(simulation.m_Scale);
	//	ripple       = std::move(simulation.ripple);
	//	lightPos     = std::move(simulation.lightPos);
	//	grid         = std::move(simulation.grid);
	//}

	//Simulation & operator=(Simulation&&p_Simulation)
	//{
	//	m_Shader     = p_Simulation.m_Shader;
	//	m_Scale      = std::move(p_Simulation.m_Scale);
	//	ripple       = std::move(p_Simulation.ripple);
	//	lightPos     = std::move(p_Simulation.lightPos);
	//	grid         = std::move(p_Simulation.grid);
	//	return *this;
	//}
	void Begin()
	{
		
	}
	void Update()
	{
	  
	}
	void Draw()
	{
		grid->Draw();
		error();
	}
};   