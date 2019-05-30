#pragma once
#include "Shaders.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ICallbacks.h"
#include "../Lines.hpp"
#include "../Ripple.hpp"

class Simulation
{
private:

	glm::vec3 lightPos = glm::vec3(-1.0f,0.2f,0.0f);
	Shader m_Shader;
	Shader m_RiplShader;
	Lines line;
	Lines line2;
	Lines line3;
	Ripple ripple;
	float m_Scale = 0.895f;
public:
	Simulation() = default;
	Simulation(Shader p_Shader)
		:
		line(glm::vec3(-100.0f,0.0f,0.0f),glm::vec3(100.0f,0.0f,0.0f)),
		line2(glm::vec3(0.0f,-100.0f,0.0f),glm::vec3(0.0f,100.0f,0.0f)),
	    line3(glm::vec3(0.0f,0.0f,-100.0f),glm::vec3(0.0f,0.0f,100.0f)),
		ripple(glm::vec3(0,0,0),1)
	{  
		m_Shader      = p_Shader;
		m_Scale       = 0.001f;
		m_RiplShader.LoadShader("VertexShaderRipple.glsl", ShaderType::VERTEX);
		m_RiplShader.LoadShader("FragmentShaderRipple.glsl", ShaderType::FRAGMENT);
		m_RiplShader.LoadProgram();
		error();
		ICallbacks::AddShader(m_RiplShader);
		ICallbacks::SetShader(m_Shader);
		line.SetShader(m_Shader);
		line2.SetShader(m_Shader);
		line3.SetShader(m_Shader);
		ripple.SetShader(m_RiplShader);
		m_RiplShader.Bind();
		m_RiplShader.setVec3("lightLocation",lightPos);
		error();
	}
	Simulation(Simulation && simulation)
	{
		m_Shader = std::move(simulation.m_Shader);
		m_Scale = std::move(simulation.m_Scale);
		line = std::move(simulation.line);
		line2 = std::move(simulation.line2);
		line3 = std::move(simulation.line3);
		ripple = std::move(simulation.ripple);
		lightPos = std::move(simulation.lightPos);
		m_RiplShader = std::move(simulation.m_RiplShader);
	}

	Simulation & operator=(Simulation&&p_Simulation)
	{
		m_Shader = p_Simulation.m_Shader;
		m_Scale = std::move(p_Simulation.m_Scale);
		line  = std::move(p_Simulation.line);
		line2 = std::move(p_Simulation.line2);
		line3 = std::move(p_Simulation.line3);
		ripple = std::move(p_Simulation.ripple);
		lightPos = std::move(p_Simulation.lightPos);
		m_RiplShader = std::move(p_Simulation.m_RiplShader);
		return *this;
	}
	void Begin()
	{
		
	}
	void Update()
	{
		if(ImGui::DragFloat3("Light Position", (float*)&lightPos, 0.002f, -10.0f, 10.0f))
		{
			m_RiplShader.Bind();
			m_RiplShader.setVec3("lightLocation", lightPos);
			error();
		}
	}
	void Draw()
	{
		line.Draw();
		line2.Draw();
		line3.Draw();
		ripple.Draw();
		error();
	}
};