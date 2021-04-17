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
        unsigned int framebuffer = 0;
        unsigned int textureColorBufferMultiSampled = 0;
        unsigned int rbo = 0;
        unsigned int intermediateFBO = 0;
        unsigned int screenTexture = 0;
        unsigned int SCREEN_WIDTH = 1280;
        unsigned int SCREEN_HEIGHT = 640;
        VertexArray vao;
        VertexArray postProcessVao;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<VertexBuffer> postProcessVbo;
        std::shared_ptr<FPCamera> cam;

        learnopengl::Shader shader;
        learnopengl::Shader postProcessShader;
        glm::vec2 offsets[100];
        float cubeVertices[108] = {
            -0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

             0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,

             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,

             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,


            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,

            -0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f,

        };
        float quadVertices[30] = {
            // positions     // colors
            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
            -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,

            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
             0.05f,  0.05f,  0.0f, 1.0f, 1.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        };
        float quadScreenVertices[24] = {   
            // vertex attributes for a quad that fills the entire screen 
            //in Normalized Device Coordinates.
            // positions   // texCoords
             -1.0f,  1.0f,  0.0f, 1.0f,
             -1.0f, -1.0f,  0.0f, 0.0f,
              1.0f, -1.0f,  1.0f, 0.0f,
             
             -1.0f,  1.0f,  0.0f, 1.0f,
              1.0f, -1.0f,  1.0f, 0.0f,
              1.0f,  1.0f,  1.0f, 1.0f
        };

    public:
        float time = 0.0f;
        SceneManager()
            :
            shader("./Shaders/learnopengl/anti_aliasing.vs", 
                "./Shaders/learnopengl/anti_aliasing.fs"),
            postProcessShader("./Shaders/learnopengl/aa_post.vs",
                "./Shaders/learnopengl/aa_post.fs")
        {
            vao.Bind();
            vbo = std::make_shared<VertexBuffer>(cubeVertices, 
                sizeof(cubeVertices)*sizeof(float));
            vbo->Bind();
            glEnableVertexArrayAttrib(vao.GetID(),0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                sizeof(float) * 3, (void*)0);
            vao.Unbind();
            setupPostProcessQuad();
            configureMsaaFramebuffer();
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
        void Draw(std::shared_ptr<Scene> scene) {
            /*vao.Bind();
            SetUniforms(shader);
            glDrawArrays(GL_TRIANGLES, 0, 36);*/
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            SetUniforms(shader);
            vao.Bind();
           CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
            glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 
                SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            // 3. now render quad with scene's visuals as its texture image
           CHECK_GL_ERROR( glBindFramebuffer(GL_FRAMEBUFFER, 0));
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            postProcessShader.use();
            postProcessShader.setInt("screenTexture", 0);
            postProcessVao.Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screenTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

    private:

        void SetUniforms(learnopengl::Shader & shader) {
            const glm::mat4 model_matrix = glm::mat4(1.0f);
            const glm::mat4 view_matrix = cam->GetView();
            const glm::mat4 projection_matrix = cam->GetProjection();
            const float time = glfwGetTime();
            shader.use();
            shader.setFloat("time", time);
            shader.setMat4("model", model_matrix);
            shader.setMat4("projection", projection_matrix);
            shader.setMat4("view", view_matrix);
        }
        void configureMsaaFramebuffer() {
            CHECK_GL_ERROR(
            glGenFramebuffers(1, &framebuffer));
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glGenTextures(1, &textureColorBufferMultiSampled);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 
                textureColorBufferMultiSampled);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB,
                SCREEN_WIDTH, SCREEN_HEIGHT, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,
                GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != 
                GL_FRAMEBUFFER_COMPLETE)
            {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" 
                    << std::endl;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            CHECK_GL_ERROR(
            glGenFramebuffers(1, &intermediateFBO));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO));
            glGenTextures(1, &screenTexture);
            glBindTexture(GL_TEXTURE_2D, screenTexture);
            CHECK_GL_ERROR(
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                GL_RGB, GL_UNSIGNED_BYTE, NULL));
            CHECK_GL_ERROR(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            CHECK_GL_ERROR(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            CHECK_GL_ERROR( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, screenTexture, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        void setupPostProcessQuad() {
            postProcessVao.Bind();
            postProcessVbo = std::make_shared<VertexBuffer>(quadScreenVertices,
                sizeof(quadScreenVertices) * sizeof(float));
            postProcessVbo->Bind();
            CHECK_GL_ERROR(
            glEnableVertexArrayAttrib(postProcessVao.GetID(), 0));
            CHECK_GL_ERROR(
            glEnableVertexArrayAttrib(postProcessVao.GetID(), 1));
            CHECK_GL_ERROR(
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                (void*)0));
            CHECK_GL_ERROR(
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                (void*)(2*sizeof(float))));
            postProcessVao.Unbind();
            
        }
    };
 }
