#pragma once
#include<glm/glm.hpp>
#include "Shaders.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Mesh.h"
class GuideLines 
{
	Mesh m;
public:
	GuideLines()
	{
	}
	GuideLines(glm::vec3 p_Pos)
	{
		m = Mesh();
		std::vector<Vertex<float>> m_Vertices;
		std::vector<unsigned int> m_Indices;
		Vertex<float> posX;
		Vertex<float> negX;
		Vertex<float> posY;
		Vertex<float> negY;
		Vertex<float> posZ;
		Vertex<float> negZ;

		posX.x = 100.0f;
		posX.r = 1.0f;
		posX.a = 1.0f;
		posY.y = 100.0f;
		posY.r = 1.0f;
		posY.a = 1.0f;
		posZ.z = 100.0f;
		posZ.r = 1.0f;
		posZ.a = 1.0f;

		negX.x = -100.0f;
		negX.r = 1.0f;
		negX.a = 1.0f;
		negY.y = -100.0f;
		negY.r = 1.0f;
		negY.a = 1.0f;
		negZ.z = -100.0f;
		negZ.r = 100.0f;
		negZ.a = 1.0f;

		m_Vertices.push_back(posX);
		m_Vertices.push_back(negX);
		m_Vertices.push_back(posY);
		m_Vertices.push_back(negY);
		m_Vertices.push_back(posZ);
		m_Vertices.push_back(negZ);
		m_Indices.push_back(0);
		m_Indices.push_back(1);
		m_Indices.push_back(2);
		m_Indices.push_back(3);
		m_Indices.push_back(4);
		m_Indices.push_back(5);
		
		
			
		    
	}
	GuideLines(GuideLines && gl)
	{

	}
	GuideLines & operator=(GuideLines && p_GuideLines)
	{
		return *this;
		
	}
	GuideLines & operator=(GuideLines & p_GuideLines)
	{		
		return *this;
	}
	
    ~GuideLines()
	{
		
	}
};