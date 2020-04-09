#pragma once
#include <GL/glew.h>
#include <gl/gl.h>
#include "Globals.h"
namespace bogong {
	class VertexBuffer
	{
	private:
		unsigned int m_ID;
		bool m_IsBound;
		size_t m_BufferSize = 0;

	public:
		VertexBuffer() = default;
		VertexBuffer(const void * p_Data, size_t p_Size, unsigned int p_Draw = GL_DYNAMIC_DRAW)
		{
			m_BufferSize = p_Size;
			CHECK_GL_ERROR(glGenBuffers(1, &m_ID));
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
			CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, p_Size, p_Data, p_Draw));
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
		size_t GetSize() const
		{
			return m_BufferSize;
		}
		void Bind() const
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
		}
		void Unbind() const
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
		unsigned int getID() const
		{
			return m_ID;
		}
	};
}