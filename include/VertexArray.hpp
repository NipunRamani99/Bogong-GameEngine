#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "Globals.h"
namespace bogong {
	class VertexArray
	{
	private:
		GLuint m_ID;
		bool m_IsBound = false;
	public:
		VertexArray() = default;
		void Bind()
		{
			if (!m_IsBound)
			{
				m_IsBound = true;
				CHECK_GL_ERROR(glGenVertexArrays(1, &m_ID));
			}
			CHECK_GL_ERROR(glBindVertexArray(m_ID));
		}
		void Unbind() {
			CHECK_GL_ERROR(glBindVertexArray(0));
		}
		GLuint GetID() const
		{
			return m_ID;
		}
	};
}