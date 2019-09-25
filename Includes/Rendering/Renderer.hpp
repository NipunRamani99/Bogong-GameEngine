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
#include <memory>
class Renderer
{
private:
	using DrawCall = std::function<void(GLenum,int)>;
	
	Shader m_Shader;
	VertexArray m_VAO;
	VertexBufferLayout  m_Layout;
	GLenum   m_DrawMode = GL_LINES;
	DrawCall  m_DrawCall = [](GLenum,size_t)-> void {};
	glm::mat4 m_Model=glm::mat4(1.0);
	
public:
	Renderer() = default;
	Renderer(const VertexBufferLayout & p_Layout)
		:
		m_Layout(p_Layout)
	{
		m_VAO = VertexArray();
	}
	/*Renderer & operator=(Renderer && p_Renderer)
	{
	    m_Mesh     = std::move(p_Renderer.m_Mesh);
		m_DrawCall = std::move(p_Renderer.m_DrawCall);
		m_Layout   = std::move(p_Renderer.m_Layout);
		m_Model    = std::move(p_Renderer.m_Model);
		m_VAO      = std::move(p_Renderer.m_VAO);
		m_DrawMode = std::move(p_Renderer.m_DrawMode);
		m_Shader   = std::move(p_Renderer.m_Shader);
		
	    return *this;
	}*/
	
	void SetShader(Shader p_Shader)
	{
		m_Shader = p_Shader;
	}
	void SetLayout(VertexBufferLayout p_Layout)
	{
		m_Layout = p_Layout;
	}
	
	void SetDrawMode(GLenum p_DrawMode)
	{
		m_DrawMode = p_DrawMode;
	}
	template<typename T>
	void RenderMesh(const std::shared_ptr<T> & mesh)
	{
		if (mesh->GetIndexBuffer().GetID() != 0)
		{
			m_DrawCall = [](GLenum DrawMode, size_t count) { glDrawElements(DrawMode, (GLsizei)count, GL_UNSIGNED_INT, 0); };
		}
		else
		{
			m_DrawCall = [](GLenum DrawMode, size_t count) { glDrawArrays(DrawMode, 0, (GLsizei)count);  };
		}
		BindBuffers(mesh);
		UnbindBuffers(mesh);
	}
	template<typename T>
	void BindBuffers(const std::shared_ptr<T> & mesh)
	{
		m_VAO.Bind();
		error();
		mesh->GetVertexBuffer().Bind();
		mesh->GetIndexBuffer().Bind();
		m_Shader.Bind();
		int stride = m_Layout.GetStride();
		size_t offset = 0;
		int i = 0;
		for (auto element : m_Layout.GetElements())
		{
			glEnableVertexAttribArray(i);
			i++;
		}
		error();
		i = 0;
		for (auto element : m_Layout.GetElements())
		{
			glVertexAttribPointer(i, element.count, element.type, element.isNormalized, sizeof(Vertex<float>), (void*)(offset));
			offset += element.count * sizeof(element.type);
			i++;
		}
		m_Shader.setMat4("model", m_Model);
		error();
		size_t count = mesh->GetCount();
		m_DrawCall(m_DrawMode, (unsigned int)count);
		error();
	}
	template<typename T>
	void UnbindBuffers(const std::shared_ptr<T> & mesh)
	{
		mesh->GetIndexBuffer().Unbind();
		mesh->GetVertexBuffer().Unbind();
		m_VAO.Unbind();
	}

};