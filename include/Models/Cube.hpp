#include "../Rendering/Mesh.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Globals.h"
#include "../Entity.hpp"
namespace bogong {
	class CubeMesh : public Mesh{
	private:
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<Texture> tex;
		glm::vec4 object_colour;
	public:
		CubeMesh(glm::vec4 colour) {
			object_colour = colour;
			float vertices[] = {
			  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	
			  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		
			   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		
			  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		
			   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
			};
			vbo = std::make_shared<VertexBuffer>(&vertices,sizeof(vertices));
			
			assert(!error());
			count = sizeof(vertices)/(sizeof(float)*6);
			VertexBufferLayout vbl;
			vbl.AddElement<float>(3);
			vbl.AddElement<float>(3);
			auto buffer1 = std::make_pair(vbo, vbl);
			m_BufferVector.push_back(buffer1);
			
		}
	
		void bind_uniforms(Shader & shader)
		{
			shader.setVec4("object_colour", object_colour);
		}
	};
	class Cube {
		std::shared_ptr<CubeMesh> mesh;
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Renderer> dots;
	public:
		Cube(glm::vec4 colour) {
			mesh = std::make_shared<CubeMesh>(colour);
			renderer = std::make_shared<Renderer>();
			dots = std::make_shared<Renderer>();
			dots->BindBuffer(mesh);
			renderer->BindBuffer(mesh);
			renderer->SetDrawMode(GL_TRIANGLES);
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