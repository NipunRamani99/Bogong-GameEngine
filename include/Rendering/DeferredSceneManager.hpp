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
#include "ReflectionScene.hpp"
#include "PBRExampleScene.h"
#include "PBRTexturedExampleScene.h"
#include <random>
#include <glm/common.hpp>
namespace bogong {

    class DefferedSceneManager {

    private:
      //  BackpackScene backpackSceneData;
      //  SponzaScene sponzaScene;
        ReflectionScene reflectionScene;
        PBRExampleScene pbrExampleScene;
        PBRTexturedExampleScene pbrTexturedExampleScene;

        unsigned int tex = 0;
        unsigned int depthMapTex = 0;
        unsigned int depthMapFBO = 0;
        unsigned int outputFBO = 0;
        unsigned int outputTex = 0;
        unsigned int outputRBO = 0;
        unsigned int ssrFBO = 0;
        unsigned int reflectedColorFBO = 0;
        unsigned int boxBlurFBO = 0;
        unsigned int ssaoFBO = 0;

        float blinnPower = 32.0f;
        float specPower = 8.0f;

        VertexArray cubeVao;
        VertexArray quadVao;
        VertexArray lightCubeVao;
        VertexArray cubeMapVao;

        std::shared_ptr<VertexBuffer> cubeVbo;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<VertexBuffer> quadVbo;
        std::shared_ptr<VertexBuffer> untexturedCubeVbo;

        std::shared_ptr<FPCamera> cam;

        learnopengl::Shader gBufferPass;
        learnopengl::Shader fboDebugPass;
        learnopengl::Shader deferredLightBoxPass;
        learnopengl::Shader deferredShadingPass;
        learnopengl::Shader ssrPass;
        learnopengl::Shader ssaoPass;
        learnopengl::Shader ssaoBlurPass;
        learnopengl::Shader boxBlurPass;
        learnopengl::Shader reflectionColorPass;
        learnopengl::Shader pbrShader;
        learnopengl::Shader pbrTextureShader;

        const unsigned int NR_LIGHTS = 32;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;
        float vigPow = 0.5f;
        float thickness = 0.01f;
        float maxDistance = 10.0;
        float resolution = 0.4;
        int  steps = 20;
        int iterationCount = 1024;
        glm::vec2 blurSize = glm::vec2{0.0f,0.0f};
        std::vector<glm::vec3> ssaoKernel;

    public:
        unsigned int gBuffer;
        unsigned int gAlbedoSpec;
        unsigned int gNormal;
        unsigned int gPos;
        unsigned int gDepth;
        unsigned int rboDepth = 0;
        unsigned int ssrTex = 0;
        unsigned int ssaoTex = 0;
        unsigned int ssaoNoiseTexture = 0;
        unsigned int boxBlurTex = 0;
        unsigned int reflectedColorTex = 0;
        bool enableNDF = true;
        bool enableG = true;
        DefferedSceneManager()
            :
            gBufferPass(G_BUFFER_VS.c_str(), G_BUFFER_FS.c_str()),
            fboDebugPass(FBO_DEBUG_VS.c_str(), FBO_DEBUG_FS.c_str()),
            deferredShadingPass(DEFERRED_SHADING_VS.c_str(), DEFERRED_SHADING_FS.c_str()),
            deferredLightBoxPass(DEFERRED_LIGHT_BOX_VS.c_str(), DEFERRED_LIGHT_BOX_FS.c_str()),
            ssrPass(DEFERRED_SHADING_VS.c_str(), SCREEN_SPACE_REFLECTION_FS.c_str()),
            boxBlurPass(DEFERRED_SHADING_VS.c_str(), DEFERRED_BOX_BLUR_FS.c_str()),
            reflectionColorPass(DEFERRED_SHADING_VS.c_str(), DEFERRED_REFLECTION_COLOR_FS.c_str()),
            ssaoPass(DEFERRED_SHADING_VS.c_str(), SCREEN_SPACE_AMBIENT_OCCLUSION_FS.c_str()),
            ssaoBlurPass(DEFERRED_SHADING_VS.c_str(), SCREEN_SPACE_AMBIENT_OCCLUSION_BLUR_FS.c_str()),
            pbrShader(PBR_VS.c_str(), PBR_FS.c_str()),
            pbrTextureShader(PBR_VS.c_str(), PBR_TEXTURED_FS.c_str()),
            pbrExampleScene(0.,0.,0.,0.,0.)
        {
            SetupOutputFBO();
            SetupScreenQuad();
            LoadLightCube();
            SetupGBuffer();
            SetupSSRFBO();
            SetupBoxBlurFBO();
            SetupReflectedColorFBO();
            SetupSSAONoiseTexture();
            SetupSSAOFBO();
            SetupSSAOKernel();
            deferredShadingPass.use();
            deferredShadingPass.setInt("gPosition", 0);
            deferredShadingPass.setInt("gNormal", 1);
            deferredShadingPass.setInt("gAlbedoSpec", 2);
           
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
            deferredShadingPass.use();
            pbrTextureShader.use();
            ImGui::Begin("Post Process");
            if (ImGui::InputFloat("Vig Power", &vigPow))
            {
                deferredShadingPass.setFloat("vigPow", vigPow);
            }
            ssrPass.use();
            if (ImGui::InputFloat("SSR Thickness", &thickness))
            {
                ssrPass.setFloat("thickness", thickness);
            }
            if (ImGui::InputFloat("SSR maxDistance", &maxDistance))
            {
                ssrPass.setFloat("maxDistance", maxDistance);
            }
            if (ImGui::InputInt("SSR steps", &steps))
            {
                ssrPass.setInt("steps", steps);
            }
            if (ImGui::InputInt("SSR iterationCount", &iterationCount))
            {
                ssrPass.setInt("iterationCount", iterationCount);
            }
            if (ImGui::InputFloat("SSR resolution", &resolution))
            {
                ssrPass.setFloat("resolution", resolution);
            }
            if (ImGui::InputFloat2("Blur Size", (float*) & blurSize))
            {
                boxBlurPass.use();
                boxBlurPass.setVec2("parameters", blurSize);
            }
            if (ImGui::Checkbox("Enable NDF", &enableNDF)) {
                pbrTextureShader.use();
                pbrTextureShader.setBool("enableNDF", enableNDF);
            }
            if (ImGui::Checkbox("Enable G", &enableG)) {
                pbrTextureShader.use();
                pbrTextureShader.setBool("enableG", enableG);
            }
            ImGui::End();
            pbrExampleScene.Update();
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
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, outputFBO));
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            pbrTextureShader.use();
            pbrTextureShader.setMat4("projection", cam->GetProjection());
            pbrTextureShader.setMat4("view", cam->GetView());
            pbrTextureShader.setVec3("camPos", cam->GetPos());

            pbrTexturedExampleScene.RenderScene(pbrTextureShader);
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return outputTex;
        }

    private:

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
        void SetupSSRFBO() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &ssrFBO));
            CHECK_GL_ERROR(glGenTextures(1, &ssrTex));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, ssrTex));
            CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, ssrFBO));
            CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssrTex, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        void SetupSSAONoiseTexture() {
            std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
            std::default_random_engine generator;
            std::vector<glm::vec3> ssaoNoise;
            for (unsigned int i = 0; i < 16; i++)
            {
                glm::vec3 noise(
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) * 2.0 - 1.0,
                    0.0f);
                ssaoNoise.push_back(noise);
            }
           CHECK_GL_ERROR( glGenTextures(1, &ssaoNoiseTexture) );
            glBindTexture(GL_TEXTURE_2D, ssaoNoiseTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
           CHECK_GL_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
        float lerp(float a, float b, float f)
        {
            return a + f * (b - a);
        }
        void SetupSSAOFBO() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &ssaoFBO));
            CHECK_GL_ERROR(glGenTextures(1, &ssaoTex));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, ssaoTex));
            CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO));
            CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTex, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        void SetupSSAOKernel() {

            std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
            std::default_random_engine generator;
            for (unsigned int i = 0; i < 64; ++i)
            {
                glm::vec3 sample(
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator)
                );
                sample = glm::normalize(sample);
                sample *= randomFloats(generator);
                float scale = (float)i / 64.0;
                
                scale = lerp(0.1f, 1.0f, scale * scale);
                sample *= scale;
                ssaoKernel.push_back(sample);
            }
            ssaoPass.use();
            for (unsigned int i = 0; i < 64; i++) {
                ssaoPass.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
            }
        }
        void SetupBoxBlurFBO() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &boxBlurFBO));
            CHECK_GL_ERROR(glGenTextures(1, &boxBlurTex));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, boxBlurTex));
            CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, boxBlurFBO));
            CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, boxBlurTex, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void SetupReflectedColorFBO() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &reflectedColorFBO));
            CHECK_GL_ERROR(glGenTextures(1, &reflectedColorTex));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, reflectedColorTex));
            CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, reflectedColorFBO));
            CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectedColorTex, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void SetupOutputFBO() {
            CHECK_GL_ERROR(glGenFramebuffers(1, &outputFBO));
            CHECK_GL_ERROR(glGenTextures(1, &outputTex));
            CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, outputTex));
            CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
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
            // depth color buffer
            glGenTextures(1, &gDepth);
            glBindTexture(GL_TEXTURE_2D, gDepth);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gDepth, 0);
           
            // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
            unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers(4, attachments);
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

        int DrawSSR(std::shared_ptr<Scene> scene) {

            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, gBuffer));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gBufferPass.use();
            gBufferPass.setMat4("projection", cam->GetProjection());
            gBufferPass.setMat4("view", cam->GetView());
            reflectionScene.RenderScene(gBufferPass);
            quadVao.Bind();
            ssaoPass.use();
            glm::mat4 projection = cam->GetProjection();
            ssaoPass.setMat4("projection", projection);
            ssaoPass.setMat4("view", cam->GetView());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPos);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gDepth);
            ssaoPass.setInt("gPosition", 0);
            ssaoPass.setInt("gNormal", 1);
            ssaoPass.setInt("texNoise", 2);
            ssaoPass.setFloat("screenWidth", SCREEN_WIDTH);
            ssaoPass.setFloat("screenHeight", SCREEN_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            //  glBindFramebuffer(GL_FRAMEBUFFER, boxBlurFBO);
            //  glClear(GL_COLOR_BUFFER_BIT );
              //ssaoBlurPass.use();
              //glActiveTexture(GL_TEXTURE0);
              //glBindTexture(GL_TEXTURE_2D, ssaoTex);
              //ssaoBlurPass.setInt("ssaoInput", 0);
              //glBindFramebuffer(GL_FRAMEBUFFER, boxBlurFBO);
              //glClear(GL_COLOR_BUFFER_BIT );
              //glDrawArrays(GL_TRIANGLES, 0, 6);

            quadVao.Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPos);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gDepth);
            ssrPass.use();
            ssrPass.setInt("gPosition", 0);
            ssrPass.setInt("gNormal", 1);
            ssrPass.setInt("gDepth", 2);
            ssrPass.setMat4("projection", projection);
            ssrPass.setMat4("view", cam->GetView());
            ssrPass.setVec3("camPos", cam->GetPos());
            glBindFramebuffer(GL_FRAMEBUFFER, ssrFBO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            quadVao.Bind();
            reflectionColorPass.use();
            glActiveTexture(GL_TEXTURE9);
            glBindTexture(GL_TEXTURE_2D, ssrTex);
            reflectionColorPass.setInt("ssrReflectedUV", 9);
            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            reflectionColorPass.setInt("gAlbedoSpec", 10);
            glBindFramebuffer(GL_FRAMEBUFFER, reflectedColorFBO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            quadVao.Bind();
            boxBlurPass.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, reflectedColorTex);
            reflectionColorPass.setInt("inputTexture", 0);
            glBindFramebuffer(GL_FRAMEBUFFER, boxBlurFBO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, outputFBO));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            deferredShadingPass.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPos);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, boxBlurTex);
            // send light relevant uniforms
            for (unsigned int i = 0; i < lightPositions.size(); i++)
            {
                deferredShadingPass.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
                deferredShadingPass.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
                // update attenuation parameters and calculate radius
                const float linear = 0.5;
                const float quadratic = 1.8;
                deferredShadingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
                deferredShadingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
            }
            deferredShadingPass.setVec3("viewPos", cam->GetPos());
            deferredShadingPass.setInt("gPosition", 0);
            deferredShadingPass.setInt("gNormal", 1);
            deferredShadingPass.setInt("gAlbedoSpec", 2);
            deferredShadingPass.setInt("ssrColorTex", 3);
            quadVao.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindTexture(GL_TEXTURE_2D, 0);
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
            return  outputTex;
        }
    };
}
