#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
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
			glGenVertexArrays(1, &m_ID);
		}
		glBindVertexArray(m_ID);
	}
	void Unbind() {
		glBindVertexArray(0);
	}
	GLuint GetID() const
	{
		return m_ID;
	}
};