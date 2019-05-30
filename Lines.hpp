#include "headers/Rendering/ShapeMesh.hpp"
#include "headers/Rendering/Renderer.hpp"
class Lines
{
	ShapeMesh m_Mesh;
	Renderer m_Renderer;
	glm::vec3 m_Point1;
	glm::vec3 m_Point2;
	VertexBufferLayout layout;
public:
	Lines() = default;
	Lines(glm::vec3 point1, glm::vec3 point2)
	{
		m_Point1 = point1;
		m_Point2 = point2;
		std::vector<Vertex<float>> vertices;
		Vertex<float> f;
		Vertex<float> f2;
		f.r = 1.0f;
		f.g = 0.0f;
		f.b = 0.0f;
		f.x = point1.x;
		f.y = point1.y;
		f.z = point1.z;
		f2.a = 1.0f;
		f2.r = 1.0f;
		f2.g = 0.0f;
		f2.b = 0.0f;
		f2.x = point2.x;
		f2.y = point2.y;
		f2.z = point2.z;
		f2.a = 1.0f;
		vertices.push_back(f);
		vertices.push_back(f2);
		m_Mesh = ShapeMesh(vertices);
		layout.AddElement<float>(3);
		layout.AddElement<float>(4);
		m_Renderer = std::move(Renderer(m_Mesh,layout));
	}
	void Draw()
	{
		m_Renderer.RenderMesh();
	}
	void SetShader(Shader m_Shader)
	{
		m_Renderer.SetShader(m_Shader);
	}

}; 