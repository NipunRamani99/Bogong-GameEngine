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
#include "Constants.h"
namespace bogong {

    class SceneManager {

    private:
        unsigned int tex = 0;
        unsigned int depthMapTex = 0;
        unsigned int depthMapFBO = 0;

        float blinnPower = 32.0f;
        float specPower = 8.0f;

        VertexArray vao;
        VertexArray cubeVao;
        VertexArray quadVao;
        VertexArray lightCubeVao;

        std::shared_ptr<VertexBuffer> cubeVbo;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<VertexBuffer> quadVbo;
        std::shared_ptr<VertexBuffer> untexturedCubeVbo;

        std::shared_ptr<FPCamera> cam;
        learnopengl::Shader shader;
        learnopengl::Shader cubeShader;
        learnopengl::Shader debugScreenShader;
        learnopengl::Shader depthShader;
        learnopengl::Shader shadowPass;
        learnopengl::Shader simpleShader;

        glm::vec2 offsets[100];
        glm::vec3 lightPos = glm::vec3(3.50, 4.0, -2.0);
        glm::vec3 cubePos = glm::vec3(0.5, 0.50, 0.0);
        glm::vec3 cubePos2 = glm::vec3(3.50, 0.50, 0.);
        glm::vec3 cubePos3 = glm::vec3(6.50, 0.5, 0.0);
        glm::mat4 cubeModel1 = glm::mat4(1.0f);
        glm::mat4 cubeModel2 = glm::mat4(1.0f);
        glm::mat4 cubeModel3 = glm::mat4(1.0f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        glm::mat4 planeModel = glm::mat4(1.0f);

        bool blinn = false;
        bool gammaCorrection = false;
        float nearPlane = 1.0f;
        float farPlane = 20.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
            nearPlane, farPlane);
        glm::mat4 lightView = glm::lookAt(lightPos,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        float time = 0.0f;
    public:
        SceneManager()
            :
            shader(ADVANCED_LIGHTNING_VERTEX_SHADER.c_str(),
                ADVANCED_LIGHTNING_FRAGMENT_SHADER.c_str()),
            cubeShader(ADVANCED_LIGHTNING_VERTEX_SHADER.c_str(),
                ADVANCED_LIGHTNING_FRAGMENT_SHADER.c_str()),
            debugScreenShader(DEBUG_QUAD_VERTEX_SHADER.c_str(),
                DEBUG_QUAD_FRAGMENT_SHADER.c_str()),
            depthShader(SIMPLE_DEPTH_VERTEX_SHADER.c_str(),
                SIMPLE_DEPTH_FRAGMENT_SHADER.c_str()),
            shadowPass(SHADOW_PASS_VERTEX_SHADER.c_str(),
                SHADOW_PASS_FRAGMENT_SHADER.c_str()),
            simpleShader(SIMPLE_VERTEX_SHADER.c_str(), SIMPLE_FRAGMENT_SHADER.c_str())
        {
            SetupPlane();
            LoadCube();
            SetupDepthMapFBO();
            SetupScreenQuad();
            cubeModel1 = glm::translate(glm::mat4(1.0f), cubePos);
            cubeModel2 = glm::translate(glm::mat4(1.0f), cubePos2);
            cubeModel3 = glm::translate(glm::mat4(1.0f), cubePos3);
            planeModel = glm::scale(glm::mat4(1.0f), glm::vec3(3.0, 1.0, 3.0));
            lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
            lightModel = glm::translate(glm::mat4(1.0f), lightPos);
            LoadLightCube();
        }

        void Update(float delta) {
            time += delta;
            ImGui::InputFloat("Camera Y", (float*)&lightPos.y, 0.01);
            if (ImGui::InputFloat3("Light Pos",(float*) &lightPos, 2)) {
                lightView = glm::lookAt(lightPos,
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
                lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
                lightModel = glm::translate(glm::mat4(1.0f), lightPos);
            }
            if (ImGui::InputFloat("Near Plane", &nearPlane) || ImGui::InputFloat("Far Plane", &farPlane)) {
                lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
                    nearPlane, farPlane);
            }
          
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

        void Draw(std::shared_ptr<Scene> scene) {
            ConfigureShaderAndMatrices();
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            RenderDepth(depthShader);
            glCullFace(GL_BACK);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ConfigureShaderAndMatrices();
            //RenderScreenQuad();
            RenderSceneWithShadow();
            RenderLightCube();
        }

        void RenderLightCube() {
            lightCubeVao.Bind();
            simpleShader.use();
            simpleShader.setMat4("model", lightModel);
            simpleShader.setMat4("projection", cam->GetProjection());
            simpleShader.setMat4("view", cam->GetView());
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        void RenderScene() {
            vao.Bind();
            shader.setInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            cubeVao.Bind();
            SetCubeUniform(shader);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        void RenderSceneWithShadow() {
            vao.Bind();
            SetUniforms(shadowPass,
                planeModel);
            shadowPass.setInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            shadowPass.setInt("shadow_map", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMapTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            cubeVao.Bind();
            SetCubeUniform(shadowPass);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            shadowPass.setMat4("model", cubeModel2);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            shadowPass.setMat4("model", cubeModel3);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        void RenderDepth(learnopengl::Shader & shader) {


            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            depthShader.use();
            depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            vao.Bind();
            depthShader.use();
            depthShader.setMat4("model", planeModel);
            depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
            cubeVao.Bind();
            depthShader.use();
            depthShader.setMat4("model", cubeModel1);
            depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            depthShader.setMat4("model", cubeModel2);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            depthShader.setMat4("model", cubeModel3);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        void RenderScreenQuad() {
            quadVao.Bind();
            debugScreenShader.use();
            debugScreenShader.setInt("outputTexture", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMapTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

    private:


        void LoadLightCube() {
            lightCubeVao.Bind();
            untexturedCubeVbo = std::make_shared<VertexBuffer>(cubeVertices, sizeof(float)
                * sizeof(cubeVertices));
            untexturedCubeVbo->Bind();
            glEnableVertexArrayAttrib(lightCubeVao.GetID(), 0);
            CHECK_GL_ERROR(
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 3, (void*)(0)));
            lightCubeVao.Unbind();
        }
        void LoadCube() {
            cubeVao.Bind();
            cubeVbo = std::make_shared<VertexBuffer>(texturedCube,
                sizeof(float) * sizeof(texturedCube));
            cubeVbo->Bind();
            glEnableVertexArrayAttrib(cubeVao.GetID(), 0);
            glEnableVertexArrayAttrib(cubeVao.GetID(), 1);
            glEnableVertexArrayAttrib(cubeVao.GetID(), 2);
            CHECK_GL_ERROR(
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(0)));
            CHECK_GL_ERROR(
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(sizeof(float) * 3)));
            CHECK_GL_ERROR(
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(sizeof(float) * 6)));
            cubeVao.Unbind();
        }

        void ConfigureShaderAndMatrices() {
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            depthShader.use();
            depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            shadowPass.use();
            shadowPass.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        }

        void SetUniforms(learnopengl::Shader & shader, const glm::mat4 model) {
            const glm::mat4 model_matrix = model;
            const glm::mat4 view_matrix = cam->GetView();
            const glm::mat4 projection_matrix = cam->GetProjection();
            const float time = glfwGetTime();
            shader.use();
            shader.setFloat("time", time);
            shader.setMat4("model", model_matrix);
            shader.setMat4("projection", projection_matrix);
            shader.setMat4("view", view_matrix);
            shader.setVec3("viewPos", cam->GetPos());
            shader.setVec3("lightPos", lightPos);
            ImGui::Begin("Bogong");
            if (ImGui::Button("Toggle Blinn"))
            {
                blinn = !blinn;
            }
            if (ImGui::Button("Toggle Gamma Correction")) {
                gammaCorrection = !gammaCorrection;
            }
            ImGui::InputFloat("Blinn Power", &blinnPower, 0.01, 0.1, 4);
            ImGui::InputFloat("Spec Power", &specPower, 0.01, 0.1, 4);
            shader.setFloat("blinnPower", blinnPower);
            shader.setFloat("specPower", specPower);
            shader.setBool("blinn", blinn);
            shader.setBool("gammaCorrection", gammaCorrection);
            ImGui::End();
        }

        void SetCubeUniform(learnopengl::Shader & shader) {
            const glm::mat4 view_matrix = cam->GetView();
            const glm::mat4 projection_matrix = cam->GetProjection();
            const float time = glfwGetTime();
            shader.use();
            shader.setFloat("time", time);
            shader.setMat4("model", cubeModel1);
            shader.setMat4("projection", projection_matrix);
            shader.setMat4("view", view_matrix);
            shader.setVec3("viewPos", cam->GetPos());
            shader.setVec3("lightPos", lightPos);
            ImGui::InputFloat("Blinn Power", &blinnPower, 0.01, 0.1, 4);
            ImGui::InputFloat("Spec Power", &specPower, 0.01, 0.1, 4);
            shader.setFloat("blinnPower", blinnPower);
            shader.setFloat("specPower", specPower);
            shader.setBool("blinn", blinn);
            shader.setBool("gammaCorrection", gammaCorrection);
        }

        void SetupDepthMapFBO() {
            CHECK_GL_ERROR(
                glGenFramebuffers(1, &depthMapFBO));
            glGenTextures(1, &depthMapTex);
            glBindTexture(GL_TEXTURE_2D, depthMapTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            CHECK_GL_ERROR(
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            CHECK_GL_ERROR(
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
            float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
            CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, depthMapTex, 0));
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }

        void SetupPlane() {
            vao.Bind();
            vbo = std::make_shared<VertexBuffer>(planeVertices,
                sizeof(planeVertices) * sizeof(float));
            vbo->Bind();
            glEnableVertexArrayAttrib(vao.GetID(), 0);
            glEnableVertexArrayAttrib(vao.GetID(), 1);
            glEnableVertexArrayAttrib(vao.GetID(), 2);
            CHECK_GL_ERROR(
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(0)));
            CHECK_GL_ERROR(
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(sizeof(float) * 3)));
            CHECK_GL_ERROR(
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(sizeof(float) * 6)));
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            int width = 0, height = 0, nChannels = 0;
            unsigned char * data = stbi_load(WOOD_TEXTURE.c_str(), &width, &height,
                &nChannels, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_image_free(data);
        }

        void SetupScreenQuad() {
            quadVao.Bind();
            quadVbo = std::make_shared<VertexBuffer>(quadScreenVertices,
                sizeof(float) * sizeof(quadScreenVertices));
            quadVbo->Bind();
            glEnableVertexArrayAttrib(quadVao.GetID(), 0);
            glEnableVertexArrayAttrib(quadVao.GetID(), 1);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                (void *)(sizeof(float) * 2));
            quadVao.Unbind();
        }
    };
}
