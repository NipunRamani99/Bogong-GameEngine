#include <vector>
#include "headers/Vertex.h"
#include "headers/Rendering/ShapeMesh.hpp"
#include "headers/Rendering/Renderer.hpp"
class Sphere
{
private:
	std::vector<Vertex<float>> m_Vertices;
	std::vector<unsigned int> m_Indices;
	VertexBufferLayout m_Layout;
	ShapeMesh m_Mesh;
	Renderer m_Renderer;
	float m_Radius = 1.0f;
	int m_Stacks = 3;
	int m_Slices = 3;
public:
	Sphere()
	{

	}
	Sphere(int p_Radius, int p_Stacks, int p_Slices)
	{
		m_Radius = p_Radius;
		m_Stacks = p_Stacks;
		m_Slices = p_Slices;
		float x, y, z, w;
		float pi = 3.141592653;
		float nx, ny, nz, lengthInv = 1.0f / m_Radius;
		float s, t;
		float sliceStep = 2 * pi / m_Slices;
		float stackStep = pi / m_Stacks;
		for (int i = 0; i <= m_Stacks; i++)
		{
			float stackAngle = pi / 2 - i * stackStep;
			float xz = m_Radius * cosf(stackAngle);
			float y = m_Radius * sinf(stackAngle);
			for (int j = 0; j <= m_Slices; j++)
			{
				float sectorAngle = j * sliceStep;
				x = xz * sinf(sectorAngle);
				z = xz * cosf(sectorAngle);
				Vertex<float> vertex;
				vertex.x = x;
				vertex.y = y;
				vertex.z = z;
				vertex.r = 1.0f;
				vertex.g = 0.5f;
				vertex.a = 1.0f;
				vertex.nx = x * lengthInv;
				vertex.ny = y * lengthInv;
				vertex.nz = z * lengthInv;
				m_Vertices.push_back(vertex);

			}
		}
		int k1, k2;
		for (int i = 0; i < m_Stacks; ++i)
		{
			k1 = i * (m_Slices + 1);     // beginning of current stack
			k2 = k1 + m_Slices + 1;      // beginning of next stack

			for (int j = 0; j < m_Slices; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					m_Indices.push_back(k1);
					m_Indices.push_back(k2);
					m_Indices.push_back(k1 + 1);
				}
				// k1+1 => k2 => k2+1
				if (i != (m_Stacks - 1))
				{
					m_Indices.push_back(k1 + 1);
					m_Indices.push_back(k2);
					m_Indices.push_back(k2 + 1);
				}
			}
		}

		m_Mesh = ShapeMesh(m_Vertices, m_Indices);
		m_Layout.AddElement<float>(3);
		m_Layout.AddElement<float>(4);
		m_Layout.AddElement<float>(3);
		m_Renderer = Renderer(m_Mesh, m_Layout);
		m_Renderer.SetDrawMode(GL_TRIANGLES);
	}
	void SetShader(Shader & p_Shader)
	{
		m_Renderer.SetShader(p_Shader);
	}
	void Draw()
	{
		m_Renderer.RenderMesh();
	}
};