#pragma once
#include "../IndexBuffer.h"
#include "../VertexBuffer.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../Vertex.h"
class Mesh
{
protected:
	std::vector<Vertex<float>> m_Vertices = std::vector<Vertex<float>>();
	std::vector<unsigned int> m_Indices = std::vector<unsigned int>();
	IndexBuffer m_IBO;
	VertexBuffer m_VBO;

	int m_Count = 0;
public:
	 Mesh()=default;
	 Mesh(Mesh && p_Mesh)
	 {
		 m_Count = std::move(p_Mesh.m_Count);
		 m_IBO = std::move(p_Mesh.m_IBO);
		 m_Indices = std::move(p_Mesh.m_Indices);
		 m_Vertices = std::move(p_Mesh.m_Vertices);
		 m_VBO = std::move(p_Mesh.m_VBO);

		 
	 }
	 Mesh & operator=(Mesh&&p_Mesh)
	 {
		 m_Vertices = std::move(p_Mesh.m_Vertices);
		 m_Indices  = std::move(p_Mesh.m_Indices);
		 m_IBO      = std::move(p_Mesh.m_IBO);
		 m_VBO      = std::move(p_Mesh.m_VBO);
		 m_Count    = std::move(p_Mesh.m_Count);
		 return *this;
	 }
	 Mesh & operator=(Mesh & p_Mesh)
	 {
		 m_Vertices =  p_Mesh.m_Vertices;
		 m_Indices  =  p_Mesh.m_Indices;
		 m_IBO      =  p_Mesh.m_IBO;
		 m_VBO      =  p_Mesh.m_VBO;
		 m_Count    =  p_Mesh.m_Count;
		 
		 return *this;
	 }
	 virtual VertexBuffer GetVertexBuffer()
	 {
		 return m_VBO;
	 }
	 virtual IndexBuffer GetIndexBuffer()
	 {
		 return m_IBO;
	 }
	 int GetCount()
	 {
		 return m_Count;
	 }
};