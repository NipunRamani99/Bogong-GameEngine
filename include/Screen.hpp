#pragma once
#include "VertexBuffer.hpp"
#include "ShaderManager.hpp"
#include "VertexArray.hpp"
#include "LearnOpenGL/Shaders.hpp"
#include "Constants.h"
namespace bogong {
    class Screen {
    private:
        learnopengl::Shader renderPass;
        learnopengl::Shader depthPass;
        std::shared_ptr<VertexBuffer> buff;
        std::shared_ptr<VertexArray> vao;
        float iTime = 0.0f;
        float nearVal = 1.0f;
        float thetaD = 45.0f;
    public:
        Screen()
            :
            renderPass(INFINITE_PLANE_VERTEX_SHADER.c_str(),
                INFINITE_PLANE_RENDER_PASS_FRAGMENT_SHADER.c_str()),
            depthPass(INFINITE_PLANE_VERTEX_SHADER.c_str(),
                INFINITE_PLANE_DEPTH_PASS_FRAGMENT_SHADER.c_str(),
                INFINITE_PLANE_DEPTH_PASS_GEOMETRIC_SHADER.c_str())
        {
            vao = std::make_shared<VertexArray>();
           
            vao->Bind();
            buff = std::make_shared<VertexBuffer>(screen_vertices, sizeof(screen_vertices));
            buff->Bind();
            CHECK_GL_ERROR(glEnableVertexAttribArray(0));
            CHECK_GL_ERROR(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
           
        }
        void ConfigureForRenderPass(glm::vec3 cam_pos, glm::vec3 cam_dir, glm::mat4 view, 
            glm::mat4 projection, glm::vec3 lightPos, float nearPlane, float farPlane, float screen_width, float screen_height) {
            renderPass.use();
            renderPass.setMat4("projection", projection);
            renderPass.setMat4("model", glm::mat4(1.0f));
            renderPass.setVec3("cam_pos", cam_pos);
            renderPass.setVec3("cam_dir", cam_dir);
            renderPass.setVec3("lightPos", lightPos);
            renderPass.setMat4("view", view);
            renderPass.setFloat("nearVal", nearPlane);
            renderPass.setFloat("farVal", farPlane);
            renderPass.setFloat("screen_width", screen_width);
            renderPass.setFloat("screen_height", screen_height);
        }
        void ConfigureForDepthPass(glm::vec3 pos, std::vector<glm::vec3> & dir, 
            std::vector<glm::mat4> & shadowProjections, float nearPlane, float farPlane,
            float screen_width, float screen_height) {
            depthPass.use();
            for (int i = 0; i < 6; i++)
                depthPass.setMat4("shadowProjections[" + std::to_string(i) + "]", shadowProjections[i]);
            for (int i = 0; i < 6; i++)
                depthPass.setVec3("shadowLightDir[" + std::to_string(i) + "]", dir[i]);
            depthPass.setMat4("model", glm::mat4(1.0f));
            depthPass.setVec3("cam_pos", pos);
            depthPass.setFloat("nearVal", nearPlane);
            depthPass.setFloat("farVal", farPlane);
            depthPass.setFloat("screen_width", screen_width);
            depthPass.setFloat("screen_height", screen_height);
        }
        void DepthPass() {
            glDisable(GL_CULL_FACE);
            vao->Bind();
            depthPass.use();
            CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
            glEnable(GL_CULL_FACE);
        }
        void RenderPass(float delta, unsigned int shadowMapCube) {
            glDisable(GL_CULL_FACE);
            vao->Bind();
            renderPass.use();
            CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE4));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapCube));
            CHECK_GL_ERROR(renderPass.setInt("depth_cube_map", 4));
            CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
            glEnable(GL_CULL_FACE);
        }
    };
}