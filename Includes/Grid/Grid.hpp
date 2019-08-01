#pragma once
#include "../Rendering/ShapeMesh.hpp"
#include "../Rendering/Renderer.hpp"
#include "../VertexBufferLayout.hpp"
class Grid
{
	ShapeMesh m_Mesh;
	Renderer m_Renderer;
public:
	Grid() {};
	Grid(glm::vec3 p_Pos)
	{
		Vertex<float> vert1(glm::vec3(0.50f,0.0f,0.5f),glm::vec4(1.0f,0.0f,0.0f,1.0f));
		Vertex<float> vert2(glm::vec3(0.50f,0.0f,-0.5f),glm::vec4(0.0f,1.0f,0.0f,1.0f));
		Vertex<float> vert3(glm::vec3(-0.50f,0.0f,-0.5f),glm::vec4(0.0f,0.0f,1.0f,1.0f));
		Vertex<float> vert4(glm::vec3(-0.50f, 0.0f,0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		std::vector<Vertex<float>> vertices;
		vertices.push_back(vert1);
		vertices.push_back(vert2);
		vertices.push_back(vert3);
		vertices.push_back(vert4);
		VertexBufferLayout vbl;
		vbl.AddElement<float>(3);
		vbl.AddElement<float>(4);
		m_Mesh = std::move(ShapeMesh(vertices));
		m_Renderer = std::move(Renderer(m_Mesh,vbl));
		m_Renderer.SetDrawMode(GL_QUADS);
	}
	void SetShader(Shader p_Shader)
	{
		m_Renderer.SetShader(p_Shader);
	}
	void Draw()
	{
		m_Renderer.RenderMesh();
	}
};