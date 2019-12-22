#include "../Rendering/Mesh.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Globals.h"
#include "../Entity.hpp"
#include "../../Imgui/imgui.h"
namespace bogong {
	class CubeMesh : public Mesh{
	private:
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<Texture> tex;
		glm::vec4 object_colour;
	public:
		CubeMesh(glm::vec4 colour) 
			:
			Mesh()
		{
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
	class Cube :public Entity
	{
		std::shared_ptr<CubeMesh> mesh;
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Renderer> dots;
	public:
		Cube(glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			 glm::vec3 pos    = glm::vec3(0.0f, 0.0f, 0.0f),
			 glm::vec3 scale  = glm::vec3(1.0f,1.0f,1.0f)
			)
			:
			Entity(pos,scale)
		{
			mesh = std::make_shared<CubeMesh>(colour);
			renderer = std::make_shared<Renderer>();
			dots = std::make_shared<Renderer>();
			dots->BindBuffer(mesh);
			renderer->BindBuffer(mesh);
			renderer->SetDrawMode(GL_TRIANGLES);
			dots->SetDrawMode(GL_POINTS);
			UpdateModel();
		}
		void Update(std::string id) {
			std::string label1 = "Input Scale##"+id;
			std::string label2 = "Input Pos##"+id;
			std::string label3 = "Input Rotate##"+id;
			if (ImGui::InputFloat3(label1.c_str(), (float*)&scale, 4))
			{
				Scale(scale);
			}
			if (ImGui::InputFloat3(label2.c_str(), (float*)&pos, 4))
			{
				Translate(pos);
			}
			if (ImGui::InputFloat3(label3.c_str(), (float*)&rotate, 4))
			{
				Rotate(rotate);
			}
		}
		void SetShader(Shader & shader) {
			renderer->SetShader(shader);
		
		}
		void Draw()
		{
			glPointSize(5.0f);
			assert(!error(), __LINE__);
			renderer->RenderMesh(mesh,model);
			assert(!error());
		}
	};
	

}