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

		VertexArray m_VAO;
		glm::mat4 m_Model = glm::mat4(1.0);
		GLenum m_DrawMode;
		Shader m_Shader;
		using DrawCall = std::function<void(GLenum, unsigned int)>;
		DrawCall  m_DrawCall = [](GLenum, int)-> void {};
	public:

		Renderer();

		template<typename T>
		void RenderMesh(const std::shared_ptr<T> & mesh);
		template<typename T>
		void RenderMesh(const std::shared_ptr<T> & mesh, glm::mat4 model);
		template<typename T>
		void BindBuffer(const std::shared_ptr<T> & mesh);
		template<typename T>
		void UnbindBuffer(const std::shared_ptr<T> & mesh);
		void SetShader(Shader p_Shader);
		void SetDrawMode(GLenum p_DrawMode);
		void SetDrawCall(DrawCall p_DrawCall);
	};
	template<typename T>
	inline void Renderer::RenderMesh(const std::shared_ptr<T>& mesh)
	{


		m_VAO.Bind();

		m_Shader.Bind();

		if (mesh->GetIndexBuffer())
		{
			m_DrawCall = [](GLenum DrawMode, unsigned int count) { glDrawElements(DrawMode, count, GL_UNSIGNED_INT, 0); };
		}
		else
		{
			m_DrawCall = [](GLenum DrawMode, unsigned int count) { glDrawArrays(DrawMode, 0, count);  };
		}

		size_t count = mesh->GetCount();

		mesh->bind_uniforms(m_Shader);
		glm::mat4 model = mesh->GetModel();
		m_Shader.setMat4("model", model);


		m_DrawCall(m_DrawMode, (unsigned int)count);

	}
	template<typename T>
	inline void Renderer::RenderMesh(const std::shared_ptr<T>& mesh, glm::mat4 model)
	{


		m_VAO.Bind();


		m_Shader.Bind();


		if (mesh->GetIndexBuffer())
		{
			m_DrawCall = [](GLenum DrawMode, unsigned int count) { glDrawElements(DrawMode, count, GL_UNSIGNED_INT, 0); };
		}
		else
		{
			m_DrawCall = [](GLenum DrawMode, unsigned int count) { glDrawArrays(DrawMode, 0, count);  };
		}

		size_t count = mesh->GetCount();
		mesh->bind_uniforms(m_Shader);
		m_Shader.setMat4("model", model);

		m_DrawCall(m_DrawMode, count);
	}
	template<typename T>
	inline void Renderer::BindBuffer(const std::shared_ptr<T>& mesh)
	{
		m_VAO.Bind();
		m_Shader.Bind();
		assert(!error());
		mesh->Bind();
		error();
	}
	template<typename T>
	inline void Renderer::UnbindBuffer(const std::shared_ptr<T>& mesh)
	{
		mesh->Unbind();
		m_VAO.Unbind();
	}
}