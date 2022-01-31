#pragma once
#include "../LearnOpenGL/Model.h"
#include <vector>
namespace bogong {
    class BackpackScene {
    private:
        learnopengl::Model backpackModel;
        std::vector<glm::vec3> objectPositions;
    public:
        BackpackScene()
            :
            backpackModel("./assets/models/backpack/backpack.obj")
        {
            objectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
            objectPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
            objectPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
            objectPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
            objectPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
            objectPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
            objectPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
            objectPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
            objectPositions.push_back(glm::vec3(3.0, -0.5, 3.0));
        }

        void Update() {

        }

        void RenderScene(learnopengl::Shader& shader) {
            shader.use();
            for (glm::vec3 objectPosition : objectPositions) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, objectPosition);
                model = glm::scale(model, glm::vec3(0.5f));
                shader.setMat4("model", model);
                backpackModel.Draw(shader);
            }
        }
    };
}
