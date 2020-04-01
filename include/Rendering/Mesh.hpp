#pragma once
#include "../IndexBuffer.h"
#include "../VertexBuffer.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../Vertex.h"
#include "../Texture.h"
#include "../Globals.h"
#include "../VertexBufferLayout.hpp"
namespace bogong {
	typedef std::pair<std::shared_ptr<VertexBuffer>, VertexBufferLayout> Buffer;
	
	class Mesh
	{
		
	protected:
		std::vector<Buffer> m_BufferVector;
		std::shared_ptr<IndexBuffer> m_IBO;
		std::vector<std::shared_ptr<Texture>> m_TexVector;
		size_t count = 0;
		glm::mat4 model = glm::mat4(1.0f);
	public:
		Mesh();
		std::vector<Buffer> & GetBuffer();
		std::shared_ptr<IndexBuffer> GetIndexBuffer();
		std::vector<std::shared_ptr<Texture>> & GetTexVector();
		glm::mat4 GetModel();
		void Bind();
		void Unbind();
		size_t GetCount();
	};
}