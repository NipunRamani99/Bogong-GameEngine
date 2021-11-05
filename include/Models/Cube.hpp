#pragma once
#include "../Rendering/Mesh.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Globals.h"
#include "../Entity.hpp"
#include "../../Imgui/imgui.h"

#include "TextureFactory.hpp"

namespace bogong {
	
	class CubeMesh : public Mesh{
	private:
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<Texture> tex;
		glm::vec4 object_colour;
		int width = 0;
		int height = 0;
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

			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 

			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 

			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 

			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 


			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 

		    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
		    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 

			};
			vbo = std::make_shared<VertexBuffer>(&vertices,sizeof(vertices));
			
			
			count = sizeof(vertices)/(sizeof(float)*6);
			VertexBufferLayout vbl;
			vbl.AddElement<float>(3);
			vbl.AddElement<float>(3);
			auto buffer1 = std::make_pair(vbo, vbl);
			m_BufferVector.push_back(buffer1);
		
			/*tex = TextureFactory::make_texture("./assets/models/crate/container2.png"); 
			assert(!error());
			auto tex2 = TextureFactory::make_texture("./assets/models/crate/container2_specular.png");
			
			m_TexVector.push_back(tex);
			m_TexVector.push_back(tex2);*/
		}
	
		void bind_uniforms(Shader & shader)
		{
			shader.setVec4("object_colour", object_colour);
		}
	};
	class TexturedCubeMesh : public Mesh {
	private:
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<Texture> tex;
		glm::vec4 object_colour;
		int width = 0;
		int height = 0;
	public:
		TexturedCubeMesh()
			:
			Mesh()
		{


			float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,

			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,


			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,

			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f

			};
			vbo = std::make_shared<VertexBuffer>(&vertices, sizeof(vertices));


			count = sizeof(vertices) / (sizeof(float) * 8);
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

	public:
		Cube(glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			 glm::vec3 pos    = glm::vec3(0.0f, 0.0f, 0.0f),
			 glm::vec3 scale  = glm::vec3(1.0f,1.0f,1.0f)
			)
			:
			Entity(pos,scale)
		{
			mesh = std::make_shared<CubeMesh>(colour);
			UpdateModel();
		}
		void Update(std::string id) {
			std::string label1 = "Input Scale##"+id;
			std::string label2 = "Input Pos##"+id;
			std::string label3 = "Input Rotate##"+id;
			if (ImGui::InputFloat3(label1.c_str(), (float*)&scale))
			{
				Scale(scale);
			}
			if (ImGui::InputFloat3(label2.c_str(), (float*)&pos))
			{
				Translate(pos);
			}
			if (ImGui::InputFloat3(label3.c_str(), (float*)&rotate))
			{
				Rotate(rotate);
			}
		}
		void SetShader(Shader & shader) {
			shader.Bind();
			shader.setInt("s", 0);
			shader.setInt("map", 1);
			
		
		}
		void Draw()
		{
			
		}
	};
	

}

/*
Cube with uv coords:
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,

			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,


			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,

			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f


*/