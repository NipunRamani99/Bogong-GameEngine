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
        unsigned int instanceVBO;
        unsigned int amount = 50000;
        glm::mat4 *modelMatrices;
        VertexArray vao;

        std::shared_ptr<VertexBuffer> vbo;

        std::shared_ptr<VertexBuffer> asteroid_vbo;

        std::shared_ptr<FPCamera> cam;

        learnopengl::Shader model_shader;
        learnopengl::Shader asteroid_shader;

        learnopengl::Model planet;
        learnopengl::Model asteroid;

        glm::vec2 offsets[100];

        float quadVertices[30] = {
            // positions     // colors
            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
            -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,

            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
             0.05f,  0.05f,  0.0f, 1.0f, 1.0f
        };

    public:
        float time = 0.0f;
        SceneManager()
            :
            model_shader("Shaders/learnopengl/simple_model_loading.vs",
                "Shaders/learnopengl/simple_model_loading.fs"),
            asteroid_shader("Shaders/learnopengl/asteroid_shader.vs",
                "Shaders/learnopengl/asteroid_shader.fs"),
            planet("./assets/models/planet/planet.obj"),
            asteroid("./assets/models/rock/rock.obj")
        {
            SetupAsteroidModel();
            SetupAsteroidBuffer();
            
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
            const glm::mat4 model_matrix = glm::mat4(1.0f);
            const glm::mat4 view_matrix = cam->GetView();
            const glm::mat4 projection_matrix = cam->GetProjection();
            const float time = glfwGetTime();
            
            SetUniforms(model_shader);
            asteroid_shader.use();
            asteroid_shader.setMat4("projection", projection_matrix);
            asteroid_shader.setMat4("view", view_matrix);
            asteroid_shader.setFloat("time", time);
            model_shader.use();
            planet.Draw(model_shader);
            asteroid_shader.use();
            glBindTexture(GL_TEXTURE_2D, asteroid.textures_loaded[0].id);
            for (unsigned int i = 0; i < asteroid.meshes.size(); i++)
            {
                glBindVertexArray(asteroid.meshes[i].VAO);
                glDrawElementsInstanced(
                    GL_TRIANGLES, asteroid.meshes[i].indices.size(), 
                    GL_UNSIGNED_INT, 0, amount
                );
            }
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
        
        void SetupAsteroidBuffer() {

            glBindVertexArray(asteroid.meshes[0].VAO);
            int pos = glGetAttribLocation(asteroid_shader.ID, "model");
            int pos1 = pos + 0;
            int pos2 = pos + 1;
            int pos3 = pos + 2;
            int pos4 = pos + 3;
            glEnableVertexAttribArray(pos1);
            glEnableVertexAttribArray(pos2);
            glEnableVertexAttribArray(pos3);
            glEnableVertexAttribArray(pos4);
            std::size_t vec4Size = sizeof(glm::vec4);
            glGenBuffers(1, &instanceVBO);
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            int size = sizeof(glm::mat4) * amount;
            CHECK_GL_ERROR(
            glBufferData(GL_ARRAY_BUFFER, size, 
                modelMatrices, GL_STATIC_DRAW));
            CHECK_GL_ERROR(
                glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE,
                    vec4Size*4, (void*)0)
            ); 
            CHECK_GL_ERROR(
                glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE,
                    vec4Size*4, (void*)(vec4Size *1))
            );
            CHECK_GL_ERROR(
                glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE,
                    vec4Size*4, (void*)(vec4Size * 2))
            );
            CHECK_GL_ERROR(
                glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE,
                    vec4Size*4, (void*)(vec4Size *3))
            );
            
            glVertexAttribDivisor(pos1, 1);
            glVertexAttribDivisor(pos2, 1);
            glVertexAttribDivisor(pos3, 1);
            glVertexAttribDivisor(pos4, 1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void SetupAsteroidModel() {
            modelMatrices = new glm::mat4[amount];
            srand(glfwGetTime()); // initialize random seed	
            float radius = 70.0f;
            float offset = 25.0f;
            for (unsigned int i = 0; i < amount; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                // 1. translation: displace along circle with 'radius' in range [-offset, offset]
                float angle = (float)i / (float)amount * 360.0f;
                float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float x = sin(angle) * radius + displacement;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float z = cos(angle) * radius + displacement;
                model = glm::translate(model, glm::vec3(x, y, z));

                // 2. scale: scale between 0.05 and 0.25f
                float scale = (rand() % 20) / 100.0f + 0.05;
                model = glm::scale(model, glm::vec3(scale));

                // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
                float rotAngle = (rand() % 360);
                model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

                // 4. now add to list of matrices
                modelMatrices[i] = model;
            }
        }
    };
 }
