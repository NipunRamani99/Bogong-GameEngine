#include "../Rendering/Mesh.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Globals.h"
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
namespace bogong {
	class CubeMesh : public Mesh{
	private:
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<Texture> tex;
	public:
		CubeMesh() {
			std::vector<Vertex<float>> vertices;
			Vertex<float> a;
			a.x = -1.0f;
			a.y = 1.0f;
			a.z = 1.0f;
			a.r = 1.0f;
			a.g = 0.0f;
			a.b = 0.0f;
			a.a = 1.0f;
			
			Vertex<float> b;
			b.x = 1.0f;
			b.y = 1.0f;
			b.z = 1.0f;
			b.r = 1.0f;
			b.g = 0.0f;
			b.b = 0.0f;
			b.a = 1.0f;

			Vertex<float> c;
			c.x = 1.0f;
			c.y = -1.0f;
			c.z = 1.0f;
			c.r = 1.0f;
			c.g = 0.0f;
			c.b = 0.0f;
			c.a = 1.0f;

			Vertex<float> d;
			d.x = -1.0f;
			d.y = -1.0f;
			d.z = 1.0f;
			d.r = 1.0f;
			d.g = 0.0f;
			d.b = 0.0f;
			d.a = 1.0f;
			Vertex<float> e;
			e.x = -1.0f;
			e.y = 1.0f;
			e.z = -1.0f;
			e.r = 1.0f;
			e.g = 0.0f;
			e.b = 0.0f;
			e.a = 1.0f;
			Vertex<float> f;
			f.x = 1.0f;
			f.y = 1.0f;
			f.z = -1.0f;
			f.r = 1.0f;
			f.g = 0.0f;
			f.b = 0.0f;
			f.a = 1.0f;
			Vertex<float> g;
			g.x = 1.0f;
			g.y = -1.0f;
			g.z = -1.0f;
			g.r = 1.0f;
			g.g = 0.0f;
			g.b = 0.0f;
			g.a = 1.0f;
			Vertex<float> h;
			h.x = -1.0f;
			h.y = -1.0f;
			h.z = -1.0f;
			h.r = 1.0f;
			h.g = 0.0f;
			h.b = 0.0f;
			h.a = 1.0f;

			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
			vertices.push_back(d);
			vertices.push_back(e);
			vertices.push_back(f);
			vertices.push_back(g);
			vertices.push_back(h);
			vbo = std::make_shared<VertexBuffer>(vertices.data(),sizeof(Vertex<float>)*vertices.size());
			std::vector<unsigned int> faces = { A,C,B,A,D,C,
											    A,H,D,A,E,H,
												E,F,G,E,G,H,
												C,G,F,C,F,B,
												D,H,C,C,H,G,
												A,B,E,B,F,E};
			m_IBO = std::make_shared<IndexBuffer>(faces.data(),faces.size()*sizeof(unsigned int));
			assert(!error());
			count = faces.size();
			VertexBufferLayout vbl;
			vbl.AddElement<float>(3);
			vbl.AddElement<float>(4);
			vbl.SetStride(sizeof(Vertex<float>));
			auto buffer1 = std::make_pair(vbo, vbl);
			m_BufferVector.push_back(buffer1);
			
		}
	};
	class Cube {
		std::shared_ptr<CubeMesh> mesh;
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Renderer> dots;
	public:
		Cube() {
			mesh = std::make_shared<CubeMesh>();
			renderer = std::make_shared<Renderer>();
			dots = std::make_shared<Renderer>();
			dots->BindBuffer(mesh);
			renderer->BindBuffer(mesh);
			renderer->SetDrawMode(GL_TRIANGLES );
			dots->SetDrawMode(GL_POINTS);
		}

		void SetShader(Shader & shader) {
			renderer->SetShader(shader);
			dots->SetShader(shader);
		}
		void Draw()
		{
			glPointSize(5.0f);
			assert(!error(), __LINE__);
			renderer->RenderMesh(mesh);
			dots->RenderMesh(mesh);
			assert(!error());
		}
	};
	

}