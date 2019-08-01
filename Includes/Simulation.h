#pragma once
#include "Shaders.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "ICallbacks.h"
#include "Lines.hpp"
#include "Ripple.hpp"
#include "Sphere.hpp"
#include "Markers/Markers.hpp"
#include "Grid/Grid.hpp"
class Simulation
{
private:

	glm::vec3 lightPos = glm::vec3(-1.0f,0.2f,0.0f);
	Shader m_Shader;
	Shader m_RiplShader;
	Shader m_MarkerShader;
	Shader m_GridShader;
	Lines line;
	Lines line2;
	Lines line3;
	Ripple ripple;
	Sphere sphere;
	Marker marker;
	Grid grid;
	float m_Scale = 0.895f;
public:
	Simulation() = default;
	Simulation(Shader p_Shader)
		:
		line(glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f)),
		line2(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f)),
		line3(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 100.0f)),
		ripple(glm::vec3(0, 0.01, 0), 2),
		sphere(1, 100, 100),
		marker(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		grid(glm::vec3(0.0f,0.0f,0.0f))
	{  
		m_Shader      = p_Shader;
		m_Scale       = 0.001f;
		m_RiplShader.LoadShader("Shaders/VertexShaderRipple.glsl", ShaderType::VERTEX);
		m_RiplShader.LoadShader("Shaders/FragmentShaderRipple.glsl", ShaderType::FRAGMENT);
		m_RiplShader.LoadProgram();
		error();
		ICallbacks::AddShader(m_RiplShader);
		ICallbacks::SetShader(m_Shader);
		m_MarkerShader.LoadShader("Shaders/MarkerVertexShader.glsl", ShaderType::VERTEX);
		m_MarkerShader.LoadShader("Shaders/MarkerFragmentShader.glsl", ShaderType::FRAGMENT);
		m_MarkerShader.LoadProgram();
		m_GridShader.LoadShader("Shaders/Grid.vert", ShaderType::VERTEX);
		m_GridShader.LoadShader("Shaders/Grid.frag", ShaderType::FRAGMENT);
		m_GridShader.LoadProgram();
		ICallbacks::AddShader(m_GridShader);
		ICallbacks::AddShader(m_MarkerShader);
		line.SetShader(m_Shader);
		line2.SetShader(m_Shader);
		line3.SetShader(m_Shader);
		ripple.SetShader(m_RiplShader);
		marker.SetShader(m_MarkerShader);
		grid.SetShader(m_GridShader);
		m_RiplShader.Bind();
		m_RiplShader.setVec3("lightLocation",lightPos);
		error();
	}
	Simulation(Simulation && simulation)
	{
		m_Shader     = std::move(simulation.m_Shader);
		m_Scale      = std::move(simulation.m_Scale);
		line         = std::move(simulation.line);
		line2        = std::move(simulation.line2);
		line3        = std::move(simulation.line3);
		ripple       = std::move(simulation.ripple);
		lightPos     = std::move(simulation.lightPos);
		m_RiplShader = std::move(simulation.m_RiplShader);
		m_MarkerShader = std::move(simulation.m_MarkerShader);
		sphere       = std::move(simulation.sphere);
		marker       = std::move(simulation.marker);
		grid         = std::move(simulation.grid);
	}

	Simulation & operator=(Simulation&&p_Simulation)
	{
		m_Shader     = p_Simulation.m_Shader;
		m_Scale      = std::move(p_Simulation.m_Scale);
		line         = std::move(p_Simulation.line);
		line2        = std::move(p_Simulation.line2);
		line3        = std::move(p_Simulation.line3);
		ripple       = std::move(p_Simulation.ripple);
		lightPos     = std::move(p_Simulation.lightPos);
		m_RiplShader = std::move(p_Simulation.m_RiplShader);
		sphere       = std::move(p_Simulation.sphere);
		marker       = std::move(p_Simulation.marker);
		grid         = std::move(p_Simulation.grid);
		m_MarkerShader = std::move(p_Simulation.m_MarkerShader);
		return *this;
	}
	void Begin()
	{
		
	}
	void Update()
	{
	  
	}
	void Draw()
	{
		grid.Draw();
		error();
	}
};   