#include "Mesh.hpp"
#include "../VertexBufferLayout.hpp"
 bogong::Mesh::Mesh() {}

 std::vector<bogong::Buffer>& bogong::Mesh::GetBuffer()
{
	return m_BufferVector;
}

 std::shared_ptr<IndexBuffer> bogong::Mesh::GetIndexBuffer()
{
	return m_IBO;
}

 std::vector<std::shared_ptr<bogong::Texture>>& bogong::Mesh::GetTexVector()
{
	return m_TexVector;
}

 glm::mat4 bogong::Mesh::GetModel()
{
	return model;
}

 void bogong::Mesh::Bind()
{
	int i = 0;
	for (auto& buffer : m_BufferVector)
	{

		for (BufferElement  & elem : buffer.second.GetElements())
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
		std::vector<BufferElement>  elements = pair.second.GetElements();
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
	if (m_IBO)
		m_IBO->Bind();
	int id = 0;
	for (auto & t : m_TexVector) {
		glActiveTexture(GL_TEXTURE0 + id);
		t->Bind();
		id++;
	}
}

 void bogong::Mesh::Unbind()
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

 size_t bogong::Mesh::GetCount()
{
	return count;
}
