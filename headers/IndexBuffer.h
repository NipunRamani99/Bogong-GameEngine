#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
class IndexBuffer
{
private:
	unsigned int m_IndexID=0;
	bool m_IsBound=false;
public:
	IndexBuffer() = default;
	IndexBuffer( unsigned int * data, unsigned int size, unsigned int draw = GL_DYNAMIC_DRAW)
	{
		glGenBuffers(1,&m_IndexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,draw);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexID);
		m_IsBound = true;
	}
	void Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_IsBound = false;
	}
	unsigned int GetID()
	{
		return m_IndexID;
	}
};