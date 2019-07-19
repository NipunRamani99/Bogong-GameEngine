#pragma once
#include "../VertexArray.hpp"
#include "Mesh.hpp"

class ShapeMesh : public Mesh
{
public:
	ShapeMesh() = default;
	ShapeMesh(std::vector<Vertex<float>> & p_Vertices)
	{
		m_Vertices = std::move(p_Vertices);
		m_VBO = VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex<float>));
		m_Count = m_Vertices.size();
	}
	ShapeMesh(std::vector<Vertex<float>> & p_Vertices, std::vector<unsigned int> & p_Indices)
	{
		m_Vertices = std::move(p_Vertices);
		m_Indices  = std::move(p_Indices);
		m_VBO = VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex<float>));
		m_IBO = IndexBuffer(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));
		m_Count = m_Indices.size();
	}
	
};