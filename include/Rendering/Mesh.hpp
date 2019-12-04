#pragma once
#include "../IndexBuffer.h"
#include "../VertexBuffer.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../Vertex.h"
namespace bogong {
	class Mesh
	{
	protected:
		std::vector<Vertex<float>> m_Vertices = std::vector<Vertex<float>>();
		std::vector<unsigned int> m_Indices = std::vector<unsigned int>();
		IndexBuffer m_IBO;
		VertexBuffer m_VBO;

		size_t m_Count = 0;
	public:
		Mesh() {}
		virtual VertexBuffer GetVertexBuffer()
		{
			return m_VBO;
		}
		virtual IndexBuffer GetIndexBuffer()
		{
			return m_IBO;
		}
		size_t GetCount()
		{
			return m_Count;
		}
	};
}