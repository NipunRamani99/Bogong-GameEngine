#pragma once
#include "Model.h"
class Cube : public Model
{
	glm::vec4 m_Color  =  glm::vec4(1.0f,1.0f,0.666f,1.0f);
public:
	Cube()
	{
	
	}
	Cube(glm::vec3 p_Position, glm::vec4 p_Color, float p_Length)
	{
		m_Color = p_Color;
		float xlength = p_Length;
		float ylength = p_Length;
		float zlength = p_Length;
		Vertex<float> vert0;
		Vertex<float> vert1;
		Vertex<float> vert2;
		Vertex<float> vert3;
		Vertex<float> vert4;
		Vertex<float> vert5;
		Vertex<float> vert6;
		Vertex<float> vert7;
		unsigned int indices[] = { 0,2,1,
								   1,2,3,
		                           4,5,6,
		                           7,6,5,
								   1,7,5,
								   1,3,7,
								   0,1,4,
								   1,5,4,
								   6,7,2,
								   7,3,2,
								   0,4,2,
								   4,6,2};
		int count = sizeof(indices) / sizeof(unsigned int);
		std::vector<unsigned int> vecIndices(indices, indices + count);

		//(0,0,0)
		vert0 = CreateVertex<float>(glm::vec3(-xlength / 2, -ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,0,0)
		vert1 = CreateVertex<float>(glm::vec3(xlength / 2, -ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(0,1,0)
		vert2 = CreateVertex<float>(glm::vec3(-xlength / 2, ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,1,0)
		vert3 = CreateVertex<float>(glm::vec3(xlength / 2, ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(0,0,1)
		vert4 = CreateVertex<float>(glm::vec3(-xlength / 2, -ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,0,1)
		vert5 = CreateVertex<float>(glm::vec3(xlength / 2, -ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		//(0,1,1)
		vert6 = CreateVertex<float>(glm::vec3(-xlength / 2, ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,1,1)
		vert7 = CreateVertex<float>(glm::vec3(xlength / 2, ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		Vertex<float>  vertexBuff[] = { vert0,vert1,vert2,vert3,vert4,vert5,vert6,vert7 };
		std::vector<Vertex<float>> vertices(vertexBuff, vertexBuff + 8);
		m_Meshes.push_back(std::move(Mesh(p_Position)));
		m_Meshes[0].SetVertices(std::move(vertices));
		m_Meshes[0].SetIndices(std::move(vecIndices));
		m_Meshes[0].SetDrawMode(GL_TRIANGLES);
	}
	
};

