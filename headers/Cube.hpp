#include "Model.h"
class Cube :public Model
{
private:
	glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
public:
	Cube()
		:
		Model()
	{

	}
	Cube(glm::vec3 p_Pos, float length, glm::vec4 p_Color)
		:
		Model()
	{
		m_Color = p_Color;
		float xlength = length;
		float ylength = length;
		float zlength = length;
		Vertex<float> vertA;
		Vertex<float> vertB;
		Vertex<float> vertC;
		Vertex<float> vertD;
		Vertex<float> vertE;
		Vertex<float> vertF;
		Vertex<float> vertG;
		Vertex<float> vertH;
		unsigned int indices[] = { 0,1,2,3,0,2,1,3,1,5,0,4,5,4,4,6,5,7,6,7,2,6,3,7 };
		int count = sizeof(indices) / sizeof(unsigned int);
		std::vector<unsigned int> vecIndices(indices, indices + count);

		//(0,0,0)
		vertA = CreateVertex<float>(glm::vec3(-xlength / 2, -ylength / 2, -zlength / 2), p_Color,glm::vec2(0,0));

		//(1,0,0)
		vertB = CreateVertex<float>(glm::vec3(xlength / 2, -ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(0,1,0)
		vertC = CreateVertex<float>(glm::vec3(-xlength / 2, ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,1,0)
		vertD = CreateVertex<float>(glm::vec3(xlength / 2, ylength / 2, -zlength / 2), p_Color, glm::vec2(0, 0));

		//(0,0,1)
		vertE = CreateVertex<float>(glm::vec3(-xlength / 2, -ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,0,1)
		vertF = CreateVertex<float>(glm::vec3(xlength / 2, -ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		//(0,1,1)
		vertG = CreateVertex<float>(glm::vec3(-xlength / 2, ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		//(1,1,1)
		vertH = CreateVertex<float>(glm::vec3(xlength / 2, ylength / 2, zlength / 2), p_Color, glm::vec2(0, 0));

		Vertex<float>  GuideVertex[] = { vertA,vertB,vertC,vertD,vertE,vertF,vertG,vertH };
		std::vector<Vertex<float>> vertices(GuideVertex,GuideVertex+8);
	
		m_Meshes.resize(1);
		m_Meshes[0] = std::move(Mesh(p_Pos));
		m_Meshes[0].SetVertices(std::move(vertices));
		m_Meshes[0].SetIndices(std::move(vecIndices));
		
	}

};
