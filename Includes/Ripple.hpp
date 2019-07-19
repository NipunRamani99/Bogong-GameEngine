#include <vector>
#include "Vertex.h"
#include "Rendering/ShapeMesh.hpp"
#include "Rendering/Renderer.hpp"
#include <glm/vec3.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
class Ripple
{
	std::vector<Vertex<float>> m_Vertices;
	std::vector<unsigned int> m_Indices;
	ShapeMesh m_Mesh;
	Renderer m_Renderer;
	VertexBufferLayout m_Layout;
	glm::vec3 m_Center = glm::vec3(0, 0, 0);
public:
	Ripple() {}
	Ripple(glm::vec3 p_Center,float width)
	{
		m_Center = p_Center;
		int steps = 100;
		float delStep = width / float(steps);
		width = float(steps / 2.0f);
		int height = steps / 2;
		//Create Vertices.
		m_Vertices.resize(steps*steps+1);
		int idx = 0;
		glm::vec3 xTangent = glm::vec3(1, 0, 0);
		glm::vec3 zTangent = glm::vec3(0, 0, 1);
		glm::vec3 res;
		float f = 1.0;
		for (int i = -steps/2; i < steps/2; i++)
		{
			float x = delStep * i;
			for (int j = -steps/2; j < steps/2; j++)
			{
				float z = j * delStep;
				float y = (float)0.03*sin(f*(x*x + z*z)*180/(float)M_PI);
				m_Vertices[idx].x = x;
				m_Vertices[idx].y = y;
				m_Vertices[idx].z = z;
				m_Vertices[idx].r = 1.0f;
				m_Vertices[idx].g = 0.5f;
				m_Vertices[idx].a = 1.0f;
				float d1 = (float)2.0f*f*x*0.03f*cos(f*((x*x + z * z) * 180.0f / (float)M_PI));
				float d2 = (float)2.0f*f*z*0.03f*cos(f*(x*x + z * z) * 180.0f /(float) M_PI);
				xTangent.y = d1;
				zTangent.y = d2;
				res = glm::normalize( glm::cross(zTangent, xTangent) );
				m_Vertices[idx].nx = res.x;
				m_Vertices[idx].ny = res.y;
				m_Vertices[idx].nz = res.z;
				idx++;

			}
		}
		/*
		* Sample Quad: For width(steps) 10
		*  10---- 11---- 12---- 13....
		*  |      |      |      |
		*  0 ---- 1 ---- 2 ---- 3....
		*  Indices pushed in Counter Clockwise Fashion like this (0,1,11,10)
		*/

		idx = 0;
		for (int j = 0; j < steps-1 ; j++)
		{
			
			for (int i = 0; i < steps-1; i++) {
				m_Indices.push_back(idx);
				m_Indices.push_back(idx + 1);
				m_Indices.push_back(idx + 1 + steps);
				m_Indices.push_back(idx + steps);
				idx++;
			}
			idx++;
		/*	m_Indices.push_back(idx );
			m_Indices.push_back(idx + width);*/
			//idx++;
		}
		
		
		m_Mesh = ShapeMesh(m_Vertices,m_Indices);
		
		m_Layout.AddElement<float>(3);
		m_Layout.AddElement<float>(4);
		m_Layout.AddElement<float>(3);
		m_Renderer = Renderer(m_Mesh,m_Layout);
		m_Renderer.SetDrawMode(GL_QUADS);
	}
	Ripple(Ripple && p_Ripple)
	{
		m_Vertices = std::move(p_Ripple.m_Vertices);
		m_Mesh = std::move(p_Ripple.m_Mesh);
		m_Renderer = std::move(p_Ripple.m_Renderer);
		m_Layout = std::move(p_Ripple.m_Layout);
	}
	Ripple & operator = (Ripple && p_Ripple)
	{
		m_Vertices = std::move(p_Ripple.m_Vertices);
		m_Mesh = std::move(p_Ripple.m_Mesh);
		m_Renderer = std::move(p_Ripple.m_Renderer);
		m_Layout = std::move(p_Ripple.m_Layout);
		return *this;
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