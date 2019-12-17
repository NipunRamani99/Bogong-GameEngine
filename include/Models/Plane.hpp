#include "../Rendering/Mesh.hpp"
#include "../Rendering/Renderer.hpp"
namespace bogong {
	class PlaneMesh : public Mesh {
	private:
		std::shared_ptr<VertexBuffer> vbo;
	public:
		PlaneMesh() {
			Vertex<float> a;
			a.x = -1.0f;
			a.y = 1.0f;
			a.r = 1.0f;
			a.a = 1.0f;
			Vertex<float> b;
			b.x = 1.0f;
			b.y = 1.0f;
			b.r = 1.0f;
			b.a = 1.0f;
			Vertex<float> c;
			c.x = 1.0f;
			c.y = -1.0f;
			c.r = 1.0f;
			c.a = 1.0f;

			Vertex<float> d;
			d.x = -1.0f;
			d.y = -1.0f;
			d.r = 1.0f;
			d.a = 1.0f;
			std::vector<Vertex<float>> vertices;
			vertices.push_back(a);
			vertices.push_back(d);
			vertices.push_back(c);
			vertices.push_back(b);

			vbo = std::make_shared<VertexBuffer>(vertices.data(), sizeof(Vertex<float>)*vertices.size());
			VertexBufferLayout vbl;
			vbl.AddElement<float>(3);
			vbl.AddElement<float>(4);
			vbl.SetStride(sizeof(Vertex<float>));
			auto buffer1 = std::make_pair(vbo, vbl);
			m_BufferVector.push_back(buffer1);
			count = 4;
		}
		void bind_uniforms(Shader & shader) {

		}
	};
	class Plane {
	private:
		std::shared_ptr <PlaneMesh> plane;
		std::shared_ptr<Renderer> renderer;
	public:
		Plane() {
			plane = std::make_shared<PlaneMesh>();
			renderer = std::make_shared<Renderer>();
			renderer->SetDrawMode(GL_QUADS);
			renderer->BindBuffer(plane);
		}
		void SetShader(Shader & shader)
		{
			renderer->SetShader(shader);
		}
		void Draw() {
			renderer->RenderMesh(plane);
			assert(!error(), __LINE__);
		}
	};
}