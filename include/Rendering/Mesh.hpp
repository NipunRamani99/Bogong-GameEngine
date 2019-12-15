#pragma once
#include "../IndexBuffer.h"
#include "../VertexBuffer.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../Vertex.h"
#include "../Texture.h"
#include "../Globals.h"
namespace bogong {
	typedef std::pair<std::shared_ptr<VertexBuffer>, VertexBufferLayout> Buffer;
	
	class Mesh
	{
		
	protected:
		std::vector<Buffer> m_BufferVector;
		std::shared_ptr<IndexBuffer> m_IBO;
		std::vector<std::shared_ptr<Texture>> m_TexVector;
		size_t count = 0;
	public:
		Mesh() {}
		std::vector<Buffer> & GetBuffer()
		{
			return m_BufferVector;
		}
		std::shared_ptr<IndexBuffer> GetIndexBuffer()
		{
			return m_IBO;
		}
		std::vector<std::shared_ptr<Texture>> & GetTexVector()
		{
			return m_TexVector;
		}
		void Bind()
		{
			int i = 0;
			for (auto& buffer : m_BufferVector)
			{
				for (auto & elem : buffer.second.GetElements())
				{
					glEnableVertexAttribArray(i);
					i++;
				}
			}
			i = 0;
			assert(!error());
			for (auto & pair : m_BufferVector)
			{
				auto buffer = pair.first.get();
				buffer->Bind();
				auto elements = pair.second.GetElements();
				int offset = 0;
				assert(!error());
				for (auto & elem : elements)
				{
					auto k = i;
					auto count = elem.count;
					auto type = elem.type;
					auto isNormalized = elem.isNormalized;
					auto stride = pair.second.GetStride();
					glVertexAttribPointer(i, count, type, isNormalized, stride, (const void *)offset);
					offset += elem.count * sizeof(float);
					i++;
				}
			}
			assert(!error());
			if (m_IBO)
				m_IBO->Bind();
			for (auto & t : m_TexVector) {
				t->Bind();
			}
		}
		void Unbind()
		{
			int i = 0;
			for (auto& buffer : m_BufferVector)
			{
				buffer.first.get()->Unbind();
				auto elems = buffer.second.GetElements();
				for (auto & v : elems)
				{
					glDisableVertexAttribArray(i);
					i++;
				}

			}
		}
		size_t GetCount()
		{
			return count;
		}
	};
}