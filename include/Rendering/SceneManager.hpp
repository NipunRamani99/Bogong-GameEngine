#pragma once
#include "Scene.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/NodeBase.hpp"
#include "Nodes/MaterialData.hpp"
#include "Nodes/DirectionalLightNode.hpp"
#include "Nodes/PointLightNode.hpp"
#include "Nodes/SpotLightNode.hpp"
#include "../ShaderManager.hpp"
#include "Framebuffer.hpp"
#include "../Screen.hpp"
#include "Rendering.hpp"
#include "../ImGuiPane.hpp"
#include "Mesh.hpp"
#include "../LearnOpenGL/Shaders.hpp"
#include "../LearnOpenGL/Model.h"
#include <vector>
#include <string>

namespace bogong {
	
	class SceneManager {

	private:
		
        VertexArray vao;
		
        std::shared_ptr<VertexBuffer> vbo;
		
        std::shared_ptr<FPCamera> cam;
		
        Program geometry_shader;
		
        learnopengl::Shader shader;

        learnopengl::Shader simple_model_shader;

        learnopengl::Shader normal_visualization_shader;

        learnopengl::Model model;

        float points[20] = {
			-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
			-0.5f, -0.5f, 0.5f, 0.2f, 0.3f// bottom-left
		};
        
	public:
		float time = 0.0f;
		SceneManager()
            :
            shader("Shaders/learnopengl/model_loading.vs", 
                "Shaders/learnopengl/model_loading.fs", 
                "Shaders/learnopengl/model_explode.gs"),
            simple_model_shader("Shaders/learnopengl/simple_model_loading.vs",
                "Shaders/learnopengl/simple_model_loading.fs"),
            normal_visualization_shader("Shaders/learnopengl/model_loading2.vs", 
                "Shaders/learnopengl/model_loading2.fs", 
                "Shaders/learnopengl/model_normal_vis.gs"),
            model("assets/models/backpack/backpack.obj")
		{
			Configuration config;	
            //Bind VAO
			vao.Bind();
            //Create Shader
			
			//Create Buffer
            vbo = std::make_shared<VertexBuffer>(points, sizeof(float) * 20);
            vbo->Bind();
            glEnableVertexArrayAttrib(vao.GetID(), 0);
            glEnableVertexArrayAttrib(vao.GetID(), 1);
            int stride = sizeof(float) * 5;
            CHECK_GL_ERROR(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                stride, (const void *)0));
            CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                stride, (const void *)(sizeof(float)*2)));
            glPointSize(4.0f);
		}
		void Update() {
		}
		void clear() {
			//dude->clear();
		}
		void init_scene(std::shared_ptr<node::NodeBase> node) {

		}
		void init_render(std::shared_ptr<node::NodeBase> node) {

		}
		void SetCamera(std::shared_ptr<FPCamera>  cam) {
			this->cam = cam;
		}
        void SetUniforms(learnopengl::Shader & shader) {
            const glm::mat4 model_matrix = glm::mat4(1.0f);
            const glm::mat4 view_matrix = cam->GetView();
            const glm::mat4 projection_matrix = cam->GetProjection();
            const float time = glfwGetTime();
            shader.use();
            shader.setMat4("model", model_matrix);
            shader.setMat4("projection", projection_matrix);
            shader.setMat4("view", view_matrix);
            shader.setFloat("time", time);
        }
        void Draw(std::shared_ptr<Scene> scene) {
            SetUniforms(shader);
            SetUniforms(simple_model_shader);
            SetUniforms(normal_visualization_shader);
            simple_model_shader.use();
            model.Draw(simple_model_shader);
            normal_visualization_shader.use();
            model.Draw(normal_visualization_shader);
        }
	};
}