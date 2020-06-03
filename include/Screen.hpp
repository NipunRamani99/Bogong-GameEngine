#pragma once
#include "VertexBuffer.hpp"
#include "ShaderManager.hpp"
#include "VertexArray.hpp"
namespace bogong {
	class Screen {
	private:
		Program prog;
		std::shared_ptr<VertexBuffer> buff;
		std::shared_ptr<VertexArray> vao;
		float iTime =0.0f; 
		float nearVal = 1.0f;
		float thetaD = 45.0f;
	public:
		Screen() {
			Configuration config;
			vao = std::make_shared<VertexArray>();
			prog = ShaderManager::GetShader("Screen",config);
			float vertices[] = {
				1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f
			};
			buff = std::make_shared<VertexBuffer>(vertices, sizeof(vertices) * sizeof(float));
		}
		void Update(glm::vec3 cam_pos, glm::vec3 cam_dir , glm::mat4 view,glm::mat4 projection) {
			prog.Bind();
			prog.setMat4("proj", projection);
			prog.setMat4("model", glm::mat4(1.0f));
			prog.setVec3("cam_pos", cam_pos);
			prog.setVec3("cam_dir", cam_dir);
			prog.setMat4("view", view);
		
		}
		void Draw(float delta) {
			vao->Bind();
			prog.Bind();
			buff->Bind();
			iTime += delta;
			prog.setFloat("iTime", iTime);
			CHECK_GL_ERROR(glEnableVertexAttribArray(0));
			CHECK_GL_ERROR(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),0));
			CHECK_GL_ERROR(glDrawArrays(GL_QUADS, 0, 4));
		}
	};
}