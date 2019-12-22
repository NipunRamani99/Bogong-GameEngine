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
		using DrawCall = std::function<void(GLenum, int)>;
		DrawCall  m_DrawCall = [](GLenum, int)-> void {};
	public:

		Renderer()
		{
			m_VAO = VertexArray();
		}

		template<typename T>
		void RenderMesh(const std::shared_ptr<T> & mesh)
		{
			assert(!error(), __LINE__);

			m_VAO.Bind();
			assert(!error(), __LINE__);

			m_Shader.Bind();
			assert(!error(), __LINE__);

			if (mesh->GetIndexBuffer())
			{
				m_DrawCall = [](GLenum DrawMode, size_t count) { glDrawElements(DrawMode, count, GL_UNSIGNED_INT, 0); };
			}
			else
			{
				m_DrawCall = [](GLenum DrawMode, size_t count) { glDrawArrays(DrawMode, 0, count);  };
			}

			size_t count = mesh->GetCount();
			assert(!error(), __LINE__);
			mesh->bind_uniforms(m_Shader);
			glm::mat4 model = mesh->GetModel();
			m_Shader.setMat4("model", model);
			assert(!error(), __LINE__);

			m_DrawCall(m_DrawMode, count);
			assert(!error(), __LINE__);
		}
		template<typename T>
		void RenderMesh(const std::shared_ptr<T> & mesh,glm::mat4 model)
		{
			assert(!error(), __LINE__);

			m_VAO.Bind();
			assert(!error(), __LINE__);

			m_Shader.Bind();
			assert(!error(), __LINE__);

			if (mesh->GetIndexBuffer())
			{
				m_DrawCall = [](GLenum DrawMode, size_t count) { glDrawElements(DrawMode, count, GL_UNSIGNED_INT, 0); };
			}
			else
			{
				m_DrawCall = [](GLenum DrawMode, size_t count) { glDrawArrays(DrawMode, 0, count);  };
			}

			size_t count = mesh->GetCount();
			assert(!error(), __LINE__);
			mesh->bind_uniforms(m_Shader);
			m_Shader.setMat4("model", model);
			assert(!error(), __LINE__);

			m_DrawCall(m_DrawMode, count);
			assert(!error(), __LINE__);
		}
		template<typename T>
		void BindBuffer(const std::shared_ptr<T> & mesh)
		{
			m_VAO.Bind();
			m_Shader.Bind();
			assert(!error());
			mesh->Bind();
			error();
		}
		template<typename T>
		void UnbindBuffer(const std::shared_ptr<T> & mesh)
		{
			mesh->Unbind();
			m_VAO.Unbind();
		}
		void SetShader(Shader p_Shader)
		{
			m_Shader = p_Shader;
		}
		void SetDrawMode(GLenum p_DrawMode)
		{
			m_DrawMode = p_DrawMode;
		}
		void SetDrawCall(DrawCall p_DrawCall)
		{
			m_DrawCall = p_DrawCall;
		}
	};
}