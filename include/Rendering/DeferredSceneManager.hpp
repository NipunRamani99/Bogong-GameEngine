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

    class DefferedSceneManager {

    private:
        BackpackScene backpackSceneData;
        SponzaScene sponzaScene;

        unsigned int tex = 0;
        unsigned int depthMapTex = 0;
        unsigned int depthMapFBO = 0;
        unsigned int outputFBO = 0;
        unsigned int outputTex = 0;
        unsigned int outputRBO = 0;

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

        const unsigned int NR_LIGHTS = 32;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;
        float vigPow = 0.5f;
    public:
        unsigned int gBuffer;
        unsigned int gAlbedoSpec;
        unsigned int gNormal;
        unsigned int gPos;
        unsigned int gDepth;
        unsigned int rboDepth = 0;
        DefferedSceneManager()
            :

            gBufferPass(G_BUFFER_VS.c_str(), G_BUFFER_FS.c_str()),
            fboDebugPass(FBO_DEBUG_VS.c_str(), FBO_DEBUG_FS.c_str()),
            deferredShadingPass(DEFERRED_SHADING_VS.c_str(), DEFERRED_SHADING_FS.c_str()),
            deferredLightBoxPass(DEFERRED_LIGHT_BOX_VS.c_str(), DEFERRED_LIGHT_BOX_FS.c_str())
        {
            SetupOutputFBO();
            SetupScreenQuad();
            LoadLightCube();
            SetupGBuffer();
         
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
            deferredShadingPass.use();
            ImGui::Begin("Post Process");
            if (ImGui::InputFloat("Vig Power", &vigPow))
            {
                deferredShadingPass.setFloat("vigPow", vigPow);
            }
            ImGui::End();
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
                    const float linear = 0.5;
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
            unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
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
    };
}
