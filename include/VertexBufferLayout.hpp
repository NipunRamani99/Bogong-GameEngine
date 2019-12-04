#pragma once
#include <vector>
#include <GL/glew.h>
#include <gl/gl.h>

struct BufferElement
{
	unsigned int type = GL_FLOAT;
	unsigned int isNormalized = GL_FALSE;
	int count = 0;
	BufferElement(unsigned int p_Type, unsigned int p_IsNormalized,int p_Count)
	{
		count = p_Count;
		isNormalized = p_IsNormalized;
		type = p_Type;

	}
};
class VertexBufferLayout
{
private:
	std::vector<BufferElement> m_Elements;
	int m_Stride = 0;
public:
	VertexBufferLayout()
	{

	}

	template<typename ElemType>
	void AddElement(int p_Count)
	{
		if (typeid(ElemType) == typeid(float))
			m_Elements.push_back({ GL_FLOAT,GL_FALSE,p_Count });
		if (typeid(ElemType) == typeid(unsigned int))
			m_Elements.push_back({ GL_UNSIGNED_INT,GL_FALSE,p_Count });

		m_Stride += sizeof(ElemType) * p_Count;
	}
	std::vector<BufferElement> & GetElements()
	{
		return m_Elements;
	}
	int GetStride()
	{
		return m_Stride;
	}
};