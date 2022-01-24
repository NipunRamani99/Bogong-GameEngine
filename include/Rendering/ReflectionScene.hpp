#pragma once
#include "../LearnOpenGL/Model.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
namespace bogong {
    class ReflectionScene {
    private:
        learnopengl::Model scene;
        glm::vec3 pos;
        glm::mat4 model;
    public:
        ReflectionScene()
            :
            scene("./assets/models/simple/reflectionscene.obj")
        {
            pos = glm::vec3(0.);
            model = glm::mat4(1.0);
        }

        void Update() {

        }

        void RenderScene(learnopengl::Shader& shader) {
            shader.use();
            shader.setMat4("model", model);
            
            scene.Draw(shader);
        }
    };
}
