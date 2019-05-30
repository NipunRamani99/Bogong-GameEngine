#pragma once 
#include <iostream>
#include "../VertexArray.hpp"
#include "../Shaders.hpp"
#include "../VertexBufferLayout.hpp"
#include "Mesh.hpp"
#include "../Globals.h"
#include <string>
#include <functional>
#include <map>
class Renderer
{
private:
	using DrawCall = std::function<void(GLenum,int)>;
	Mesh m_Mesh;
	Shader m_Shader;
	VertexArray m_VAO;
	VertexBufferLayout  m_Layout;
	GLenum   m_DrawMode=GL_LINES;
	DrawCall  m_DrawCall = [](GLenum,int)-> void {};
	glm::mat4 m_Model=glm::mat4(1.0);
	
public:
	Renderer() = default;
	
	Renderer(Mesh & p_Mesh, VertexBufferLayout & p_Layout)
		:
		m_Layout(p_Layout)
	{
		m_Mesh = p_Mesh;
		if (m_Mesh.GetIndexBuffer().GetID() != 0)
		{
			m_DrawCall = [](GLenum DrawMode,int count) { glDrawElements(DrawMode, count, GL_UNSIGNED_INT, 0); };
		}
		else
		{
			m_DrawCall = [](GLenum DrawMode,int count) { glDrawArrays(DrawMode, 0,count);  };
		}
		m_VAO = VertexArray();
	}
	Renderer & operator=(Renderer && p_Renderer)
	{
	    m_Mesh     = std::move(p_Renderer.m_Mesh);
		m_DrawCall = std::move(p_Renderer.m_DrawCall);
		m_Layout   = std::move(p_Renderer.m_Layout);
		m_Model    = std::move(p_Renderer.m_Model);
		m_VAO      = std::move(p_Renderer.m_VAO);
		m_DrawMode = std::move(p_Renderer.m_DrawMode);
		m_Shader   = std::move(p_Renderer.m_Shader);
		
	    return *this;
	}
	Renderer(Renderer && renderer)
		:
		m_Mesh(std::move(renderer.m_Mesh)),
		m_Layout(std::move(renderer.m_Layout)),
		m_VAO(std::move(renderer.m_VAO)),
		m_Shader(std::move(renderer.m_Shader)),
		m_DrawMode(std::move(renderer.m_DrawMode)),
		m_DrawCall(std::move(renderer.m_DrawCall))
	{
		
	}
	void SetShader(Shader p_Shader)
	{
		m_Shader = p_Shader;
	}
	void SetLayout(VertexBufferLayout p_Layout)
	{
		m_Layout = p_Layout;
	}
	void SetMesh(Mesh & p_Mesh)
	{
		m_Mesh  = p_Mesh;
	}
	void SetDrawMode(GLenum p_DrawMode)
	{
		m_DrawMode = p_DrawMode;
	}
	void RenderMesh();
	void BindBuffers();
	void UnbindBuffers();

};