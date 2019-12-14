#pragma once
#include "../Rendering/ShapeMesh.hpp"
#include "../Rendering/Renderer.hpp"
#include "../VertexBufferLayout.hpp"
#include <memory>
namespace bogong {
	class Grid
	{
		std::shared_ptr<ShapeMesh> m_Mesh;
		std::shared_ptr<Renderer> m_Renderer;
	public:
		Grid() {};
		Grid(glm::vec3 p_Pos)
		{
			float zmin = -5.0f;
			float zmax = 5.0f;
			float xmin = -5.0f;
			float xmax = 5.0f;
			float freq = 0.5f;
			std::vector<Vertex<float>> vertices;
			glm::vec4 color = glm::vec4(0.27450, 0.27450, 0.27450, 1.0);
			for (float z = zmin; z <= zmax; z += freq)
			{
				Vertex<float> vertex1(glm::vec3(xmin, 0, z), color, glm::vec3(0, 0, 0), glm::vec2(0, 0));
				Vertex<float> vertex2(glm::vec3(xmax, 0, z), color, glm::vec3(0, 0, 0), glm::vec2(0, 0));
				vertices.push_back(vertex1);
				vertices.push_back(vertex2);
			}
			for (float x = xmin; x <= xmax; x += freq)
			{
				Vertex<float> vertex1(glm::vec3(x, 0, zmin), color, glm::vec3(0, 0, 0), glm::vec2(0, 0));
				Vertex<float> vertex2(glm::vec3(x, 0, zmax), color, glm::vec3(0, 0, 0), glm::vec2(0, 0));
				vertices.push_back(vertex1);
				vertices.push_back(vertex2);
			}
			VertexBufferLayout vbl;
			vbl.AddElement<float>(3);
			vbl.AddElement<float>(4);
			m_Mesh = std::make_shared<ShapeMesh>(ShapeMesh(vertices));
			m_Renderer = std::make_shared<Renderer>(Renderer(vbl));
			m_Renderer->SetDrawMode(GL_LINES);
			m_Renderer->BindBuffers(m_Mesh);
		}
		void SetShader(Shader p_Shader)
		{
			m_Renderer->SetShader(p_Shader);
		}
		void Draw()
		{
			glPushAttrib(GL_LINE_BIT);
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(1.0f);
			m_Renderer->RenderMesh(m_Mesh);
			glPopAttrib();
		}
	};
}