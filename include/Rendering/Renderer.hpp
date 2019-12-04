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
namespace bogong {
	class Renderer
	{
	private:
		using DrawCall = std::function<void(GLenum, int)>;

		bogong::Shader m_Shader;
		VertexArray m_VAO;
		VertexBufferLayout  m_Layout;
		GLenum   m_DrawMode = GL_LINES;
		DrawCall  m_DrawCall = [](GLenum, size_t)-> void {};
		glm::mat4 m_Model = glm::mat4(1.0);

	public:
		Renderer();
		Renderer(const VertexBufferLayout & p_Layout)
			:
			m_Layout(p_Layout)
		{
			m_VAO = VertexArray();
		}


		void SetShader(bogong::Shader p_Shader)
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
			m_VAO.Bind();
			m_Shader.Bind();
			m_Shader.setMat4("model", m_Model);
			error();
			size_t count = mesh->GetCount();
			m_DrawCall(m_DrawMode, (unsigned int)count);
			error();
		}
		template<typename T>
		void BindBuffers(const std::shared_ptr<T> & mesh)
		{
			m_VAO.Bind();
			error();
			mesh->GetVertexBuffer().Bind();
			mesh->GetIndexBuffer().Bind();
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
}