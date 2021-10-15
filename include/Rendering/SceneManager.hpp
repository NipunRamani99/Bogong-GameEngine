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

        VertexArray planeVao;
        VertexArray cubeVao;
        VertexArray quadVao;
        VertexArray lightCubeVao;
        VertexArray cubeMapVao;

        std::shared_ptr<VertexBuffer> cubeMapVbo;
        std::shared_ptr<VertexBuffer> cubeVbo;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<VertexBuffer> quadVbo;
        std::shared_ptr<VertexBuffer> untexturedCubeVbo;

        std::shared_ptr<FPCamera> cam;
        learnopengl::Shader shader;
        learnopengl::Shader cubeShader;
        learnopengl::Shader debugScreenShader;
        learnopengl::Shader simpleShader;
        learnopengl::Shader pointShadowDepthPass;
        learnopengl::Shader pointShadowRenderPass;
        learnopengl::Shader pointShadowDepthDebug;


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
        float farPlane = 25.f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 
            float(SHADOW_WIDTH)/float(SHADOW_HEIGHT), nearPlane, farPlane);
        std::vector<glm::mat4> shadowTransforms;
        float time = 0.0f;
        unsigned int depthCubeMap = 0;
    public:
        SceneManager()
            :
            shader(ADVANCED_LIGHTNING_VERTEX_SHADER.c_str(),
                ADVANCED_LIGHTNING_FRAGMENT_SHADER.c_str()),
            cubeShader(ADVANCED_LIGHTNING_VERTEX_SHADER.c_str(),
                ADVANCED_LIGHTNING_FRAGMENT_SHADER.c_str()),
            debugScreenShader(DEBUG_QUAD_VERTEX_SHADER.c_str(),
                DEBUG_QUAD_FRAGMENT_SHADER.c_str()),
            simpleShader(SIMPLE_VERTEX_SHADER.c_str(), SIMPLE_FRAGMENT_SHADER.c_str()),
            pointShadowDepthPass(POINT_SHADOW_DEPTH_VERTEX_SHADER.c_str(),
                POINT_SHADOW_DEPTH_FRAGMENT_SHADER.c_str(), 
                POINT_SHADOW_DEPTH_GEOMETRY_SHADER.c_str()),
            pointShadowRenderPass(POINT_SHADOW_PASS_VERTEX_SHADER.c_str(), 
                POINT_SHADOW_PASS_FRAGMENT_SHADER.c_str()),
            pointShadowDepthDebug(POINT_SHADOW_DEPTH_DEBUG_VERTEX_SHADER.c_str(),
                POINT_SHADOW_DEPTH_DEBUG_FRAGMENT_SHADER.c_str())
        {
            CHECK_GL_ERROR(SetupDepthCubeMap());
            SetupPlane();
            LoadCube();
            SetupScreenQuad();
            SetupModels();
            SetupShadowMatrices();
            LoadLightCube();
            SetupCubeMap();
            lightModel = glm::translate(glm::mat4(1.0f), lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
        }

        void Update(float delta) {
            time += delta;
            ImGui::InputFloat("Camera Y", (float*)&lightPos.y, 0.01);
            if (ImGui::InputFloat3("Light Pos",(float*) &lightPos, 2)) {
                shadowTransforms.clear();
                shadowTransforms.push_back(shadowProj *
                    glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
                shadowTransforms.push_back(shadowProj *
                    glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
                shadowTransforms.push_back(shadowProj *
                    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
                shadowTransforms.push_back(shadowProj *
                    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
                shadowTransforms.push_back(shadowProj *
                    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
                shadowTransforms.push_back(shadowProj *
                    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
                lightModel = glm::translate(glm::mat4(1.0f), lightPos);
                lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
            }
            if (ImGui::InputFloat("Near Plane", &nearPlane) || ImGui::InputFloat("Far Plane", &farPlane)) {
                shadowProj = glm::perspective(glm::radians(90.0f),
                    float(SHADOW_WIDTH) / float(SHADOW_HEIGHT), nearPlane, farPlane);
            }
        }

        void SetupShadowMatrices() {
            shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
            shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
            shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
        }
        
        void SetupModels() {
            cubeModel1 = glm::translate(glm::mat4(1.0f), cubePos);
            cubeModel2 = glm::translate(glm::mat4(1.0f), cubePos2);
            cubeModel3 = glm::translate(glm::mat4(1.0f), cubePos3);
            planeModel = glm::scale(glm::mat4(1.0f), glm::vec3(3.0, 1.0, 3.0));
            lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
            lightModel = glm::translate(glm::mat4(1.0f), lightPos);
        }

        void clear() {
        }

        void init_scene(std::shared_ptr<node::NodeBase> node) {
        }

        void init_render(std::shared_ptr<node::NodeBase> node) {
        }

        void SetCamera(std::shared_ptr<FPCamera>  cam) {
            this->cam = cam;
        }

        void Draw(std::shared_ptr<Scene> scene) {
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            pointShadowDepthPass.use();
            for (unsigned int i = 0; i < 6; ++i)
                pointShadowDepthPass.setMat4("shadowProjections[" + std::to_string(i) + "]", shadowTransforms[i]);
            pointShadowDepthPass.setFloat("far_plane", farPlane);
            pointShadowDepthPass.setVec3("light_pos", lightPos);
            RenderShit(pointShadowDepthPass);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ConfigureShaderMatricesForRenderPass();
            RenderShit(pointShadowRenderPass);
            renderLightCube(simpleShader);
        }

    private:

        void ConfigureShaderMatricesForDepthPass() {
            pointShadowDepthPass.use();
            for (int i = 0; i < 6; i++) {
                pointShadowDepthPass.setMat4("shadowProjections[" + 
                    std::to_string(i) + "]", shadowTransforms[i]);
            }
            pointShadowDepthPass.setFloat("far_plane", farPlane);
            pointShadowDepthPass.setVec3("light_pos", lightPos);
        }

        void ConfigureShaderMatricesForRenderPass() {
            CHECK_GL_ERROR(pointShadowRenderPass.use());
            CHECK_GL_ERROR(pointShadowRenderPass.setFloat("far_plane", farPlane));
            CHECK_GL_ERROR(pointShadowRenderPass.setMat4("projection", cam->GetProjection()));
            CHECK_GL_ERROR(pointShadowRenderPass.setMat4("view", cam->GetView()));
            CHECK_GL_ERROR(pointShadowRenderPass.setVec3("viewPos", cam->GetPos()));
            CHECK_GL_ERROR(pointShadowRenderPass.setVec3("lightPos", lightPos));
            CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, tex));
            CHECK_GL_ERROR(pointShadowRenderPass.setInt("diffuse_texture", 0));
            CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE1));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap));
            CHECK_GL_ERROR(pointShadowRenderPass.setInt("depth_cube_map", 1));
        }

        void RenderShit(learnopengl::Shader & shader) {
            shader.use();
            planeVao.Bind();
            shader.setMat4("model", planeModel);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            cubeVao.Bind();
            shader.setMat4("model", cubeModel1);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            shader.setMat4("model", cubeModel2);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            shader.setMat4("model", cubeModel3);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        void RenderCubeMap(learnopengl::Shader & shader) {
            glDepthFunc(GL_LEQUAL);
            cubeMapVao.Bind();
            shader.use();
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
            shader.setInt("cubemap", 1);
            glm::mat4 view = glm::mat4(glm::mat3(cam->GetView()));
            shader.setMat4("view", view);
            shader.setMat4("projection", cam->GetProjection());
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glDepthFunc(GL_LESS);
        }

        void renderLightCube(learnopengl::Shader & shader) {
            lightCubeVao.Bind();
            shader.use();
            shader.setMat4("model", lightModel);
            shader.setMat4("projection", cam->GetProjection());
            shader.setMat4("view", cam->GetView());
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
            planeVao.Bind();
            vbo = std::make_shared<VertexBuffer>(planeVertices,
                sizeof(planeVertices) * sizeof(float));
            vbo->Bind();
            glEnableVertexArrayAttrib(planeVao.GetID(), 0);
            glEnableVertexArrayAttrib(planeVao.GetID(), 1);
            glEnableVertexArrayAttrib(planeVao.GetID(), 2);
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
            glBindTexture(GL_TEXTURE_2D, 0);
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

        
        void SetupDepthCubeMap() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &depthMapFBO));
            CHECK_GL_ERROR(glGenTextures(1, &depthCubeMap));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap));
            for (unsigned int i = 0; i < 6; ++i)
                CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_WIDTH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
            CHECK_GL_ERROR(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0));
            CHECK_GL_ERROR(glDrawBuffer(GL_NONE));
            CHECK_GL_ERROR(glReadBuffer(GL_NONE));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }

        void SetupCubeMap() {
            cubeMapVao.Bind();
            cubeMapVbo = std::make_shared<VertexBuffer>(skyboxVertices,
                sizeof(skyboxVertices));
            cubeMapVbo->Bind();
            glEnableVertexArrayAttrib(cubeMapVao.GetID(), 0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                (void*)0);
            cubeMapVao.Unbind();
        }
    };
}
