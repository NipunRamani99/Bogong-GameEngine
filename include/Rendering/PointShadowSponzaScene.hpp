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
#include "RenderConstants.h"
#include "SponzaScene.h"
#include "NanosuitScene.h"
#include "BackpackScene.hpp"
namespace bogong {

    class PointShadowSponzaScene {

    private:
        Screen screen;
      //  SponzaScene sponzaScene;
      //  NanosuitScene nanosuitScene;
        BackpackScene backpackSceneData;

        unsigned int tex = 0;
        unsigned int depthMapTex = 0;
        unsigned int depthMapFBO = 0;
        unsigned int outputFBO = 0;
        unsigned int outputTex = 0;
        unsigned int outputRBO = 0;




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
        learnopengl::Shader pointShadowDepthPassTBN;
        learnopengl::Shader gBufferPass;
        learnopengl::Shader fboDebugPass;
        learnopengl::Shader deferredLightBoxPass;
        learnopengl::Shader deferredShadingPass;


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
        float nearPlane = 0.10f;
        float farPlane = 25.f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 
            float(SHADOW_WIDTH)/float(SHADOW_HEIGHT), nearPlane, farPlane);
        std::vector<glm::mat4> shadowTransforms;
        std::vector<glm::vec3> shadowDirections;
        float time = 0.0f;
        unsigned int depthCubeMap = 0;
        float phase = 0.0f;
        float amplitude = 4.0f;
        const unsigned int NR_LIGHTS = 32;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;
    public:
        unsigned int gBuffer;
        unsigned int gAlbedoSpec;
        unsigned int gNormal;
        unsigned int gPos;
        unsigned int rboDepth = 0;
        PointShadowSponzaScene()
            :
            screen(),
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
                POINT_SHADOW_DEPTH_DEBUG_FRAGMENT_SHADER.c_str()),
            pointShadowDepthPassTBN(POINT_SHADOW_PASS_TBN_VERTEX_SHADER.c_str(),
                POINT_SHADOW_PASS_TBN_FRAGMENT_SHADER.c_str()),
            gBufferPass(G_BUFFER_VS.c_str(), G_BUFFER_FS.c_str()),
            fboDebugPass(FBO_DEBUG_VS.c_str(), FBO_DEBUG_FS.c_str()),
            deferredShadingPass(DEFERRED_SHADING_VS.c_str(), DEFERRED_SHADING_FS.c_str()),
            deferredLightBoxPass(DEFERRED_LIGHT_BOX_VS.c_str(), DEFERRED_LIGHT_BOX_FS.c_str())
        {
            CHECK_GL_ERROR(SetupDepthCubeMap());
            SetupOutputFBO();
            SetupPlane();
            LoadCube();
            SetupScreenQuad();
            SetupModels();
            SetupShadowMatrices();
            LoadLightCube();
            SetupCubeMap();
            SetupGBuffer();
            lightModel = glm::translate(glm::mat4(1.0f), lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
            
            deferredShadingPass.use();
            deferredShadingPass.setInt("gPosition", 0);
            deferredShadingPass.setInt("gNormal", 1);
            deferredShadingPass.setInt("gAlbedoSpec", 2);
            srand(13);
            for (unsigned int i = 0; i < NR_LIGHTS; i++)
            {
                // calculate slightly random offsets
                float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
                float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
                float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
                lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
                // also calculate random color
                float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
                float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
                float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
                lightColors.push_back(glm::vec3(rColor, gColor, bColor));
            }
        }

        void Update(float delta) {
            ImGui::Begin("Update");
            time += delta;
            lightPos.x = phase + amplitude*sin(time);
            lightModel = glm::translate(glm::mat4(1.0f), lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.25, 0.25, 0.25));
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
            ImGui::InputFloat("Phase", &phase, 0.01);
            ImGui::InputFloat("Amplitude", &amplitude, 0.01);

            
            if (ImGui::InputFloat3("Light Pos",(float*) &lightPos)) {
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
            ImGui::End();
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
            shadowDirections.push_back(glm::vec3(1.0, 0., 0.));
            shadowDirections.push_back(glm::vec3(1.0, 0., 0.));
            shadowDirections.push_back(glm::vec3(0.0, 1., 0.));
            shadowDirections.push_back(glm::vec3(1.0, -1., 0.));
            shadowDirections.push_back(glm::vec3(0.0, 0., 1.0));
            shadowDirections.push_back(glm::vec3(0.0, 0., -1.0));
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

        int Draw(std::shared_ptr<Scene> scene) {
   
           CHECK_GL_ERROR( glBindFramebuffer(GL_FRAMEBUFFER, gBuffer) );
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                gBufferPass.use();
                gBufferPass.setMat4("projection", cam->GetProjection());
                gBufferPass.setMat4("view", cam->GetView());
                backpackSceneData.RenderScene(gBufferPass);
           
           CHECK_GL_ERROR( glBindFramebuffer(GL_FRAMEBUFFER, outputFBO));
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                deferredShadingPass.use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gPos);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, gNormal);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
                // send light relevant uniforms
                for (unsigned int i = 0; i < lightPositions.size(); i++)
                {
                    deferredShadingPass.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
                    deferredShadingPass.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
                    // update attenuation parameters and calculate radius
                    const float linear = 0.7;
                    const float quadratic = 1.8;
                    deferredShadingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
                    deferredShadingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
                }
                deferredShadingPass.setVec3("viewPos", cam->GetPos());
                quadVao.Bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outputFBO);

                glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
                glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);

                deferredLightBoxPass.use();
                deferredLightBoxPass.setMat4("projection", cam->GetProjection());
                deferredLightBoxPass.setMat4("view", cam->GetView());
                for (unsigned int i = 0; i < lightPositions.size(); i++)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, lightPositions[i]);
                    model = glm::scale(model, glm::vec3(0.125f));
                    deferredLightBoxPass.setMat4("model", model);
                    deferredLightBoxPass.setVec3("lightColor", lightColors[i]);
                    renderLightCube(deferredLightBoxPass, model);
                }
 
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return outputTex;
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

        void ConfigureShaderMatricesForRenderPass(learnopengl::Shader & shader) {
            CHECK_GL_ERROR(shader.use());
            CHECK_GL_ERROR(shader.setFloat("far_plane", farPlane));
            CHECK_GL_ERROR(shader.setMat4("projection", cam->GetProjection()));
            CHECK_GL_ERROR(shader.setMat4("view", cam->GetView()));
            CHECK_GL_ERROR(shader.setVec3("viewPos", cam->GetPos()));
            CHECK_GL_ERROR(shader.setVec3("lightPos", lightPos));
            //CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
            //CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, tex));
          //  CHECK_GL_ERROR(shader.setInt("diffuse_texture", 0));
            CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE4));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap));
            CHECK_GL_ERROR(shader.setInt("depth_cube_map", 4));
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
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
            shader.setInt("cubemap", 4);
            glm::mat4 view = glm::mat4(glm::mat3(cam->GetView()));
            shader.setMat4("view", view);
            shader.setMat4("projection", cam->GetProjection());
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glDepthFunc(GL_LESS);
        }

        void renderLightCube(learnopengl::Shader & shader, glm::mat4 model) {
            lightCubeVao.Bind();
            shader.use();
            shader.setMat4("model", model);
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

        void SetupOutputFBO() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &outputFBO));
            CHECK_GL_ERROR(glGenTextures(1, &outputTex));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, outputTex));
            CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, outputFBO));
            CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTex, 0));
            glGenRenderbuffers(1, &outputRBO);
            glBindRenderbuffer(GL_RENDERBUFFER, outputRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, outputRBO);
            
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

        void SetupGBuffer() {
            glGenFramebuffers(1, &gBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            // position color buffer
            glGenTextures(1, &gPos);
            glBindTexture(GL_TEXTURE_2D, gPos);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);
            // normal color buffer
            glGenTextures(1, &gNormal);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
            // color + specular color buffer
            glGenTextures(1, &gAlbedoSpec);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
            // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
            unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, attachments);
            // create and attach depth buffer (renderbuffer)
            CHECK_GL_ERROR(glGenRenderbuffers(1, &rboDepth));
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
            // finally check if framebuffer is complete
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
    };
}
