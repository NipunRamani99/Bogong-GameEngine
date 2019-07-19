#pragma once
#include "Model.h"
#include "Cube.h"
#include "Shaders.hpp"
#include <glm/gtx/transform.hpp>
#include <algorithm>

class LightSource
{
private:
	glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	Cube cube;
	Shader m_Shader;
public:
	LightSource(LightSource && p_LightSource)
	{
		cube = std::move(p_LightSource.cube);
		color = std::move(p_LightSource.color);
	}
	LightSource(glm::vec3 p_Pos, glm::vec4 p_Color)
		:
		cube(p_Pos,p_Color, 0.04f)
	{
		
	}
	LightSource & operator=(LightSource && p_LightSrc)
	{
		color = std::move(p_LightSrc.color);
		cube = std::move(p_LightSrc.cube);
		m_Shader = std::move(p_LightSrc.m_Shader);
		return *this;
	}
	LightSource() = default;
	void SetShader(Shader p_Shader)
	{
		p_Shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		cube.SetShader(p_Shader);	
	}
	void Apply()
	{
		cube.Draw();
	}
};