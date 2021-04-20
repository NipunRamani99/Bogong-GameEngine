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
        unsigned int tex = 0;
        unsigned int SCREEN_WIDTH = 1280;
        unsigned int SCREEN_HEIGHT = 640;
        float blinnPower = 32.0f;
        float specPower = 8.0f;
        VertexArray vao;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<FPCamera> cam;
        learnopengl::Shader shader;
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
        float planeVertices[48] = {
            // positions            // normals         // texcoords
             10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
            -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
             10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
             10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        };
        glm::vec3 lightPos = glm::vec3(0.0, 0.0, 0.0);
        bool blinn = false;
        bool gammaCorrection = false;
    public:
        float time = 0.0f;
        SceneManager()
            :
            shader("./Shaders/learnopengl/advanced_lighting.vs",
                "./Shaders/learnopengl/advanced_lighting.fs")
        {
            vao.Bind();
            vbo = std::make_shared<VertexBuffer>(planeVertices, 
                sizeof(planeVertices)*sizeof(float));
            vbo->Bind();
            glEnableVertexArrayAttrib(vao.GetID(), 0);
            glEnableVertexArrayAttrib(vao.GetID(), 1);
            glEnableVertexArrayAttrib(vao.GetID(), 2);
            CHECK_GL_ERROR(
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(0)));
            CHECK_GL_ERROR(
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(sizeof(float)*3)));
            CHECK_GL_ERROR(
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                    sizeof(float) * 8, (void*)(sizeof(float)*6)));
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            int width = 0, height = 0, nChannels = 0;
            unsigned char * data = stbi_load("./assets/models/nicewood.png", &width, &height,
                &nChannels, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_image_free(data);
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
            vao.Bind();
            SetUniforms(shader);
            shader.setInt("texture_diffuse1", 0);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
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
            shader.setVec3("viewPos", cam->GetPos());
            shader.setVec3("lightPos", lightPos);
            ImGui::Begin("Bogong");
            if(ImGui::Button("Toggle Blinn"))
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
    };
 }
