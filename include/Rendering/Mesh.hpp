#pragma once
#include "../IndexBuffer.h"
#include "../VertexBuffer.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../Vertex.h"
#include "../Texture.h"
namespace bogong {
	typedef std::pair<std::shared_ptr<VertexBuffer>, VertexBufferLayout> Buffer;
	class Mesh
	{
		
	protected:
		std::vector<Buffer> m_BufferVector;
		IndexBuffer m_IBO;
		std::vector<std::shared_ptr<Texture>> m_TexVector;
		size_t m_Count = 0;
	public:
		Mesh() {}
		std::vector<Buffer> & GetBuffer()
		{
			return m_BufferVector;
		}
		IndexBuffer GetIndexBuffer()
		{
			return m_IBO;
		}
		std::vector<std::shared_ptr<Texture>> & GetTexVector()
		{
			return m_TexVector;
		}
		size_t GetCount()
		{
			return m_Count;
		}
	};
}