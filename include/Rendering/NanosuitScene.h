#pragma once
#include "../LearnOpenGL/Model.h"
namespace bogong {
    class NanosuitScene {
    private:
        learnopengl::Model suitModel;
        glm::mat4 model;
    public:
        NanosuitScene()
            :
            suitModel("./assets/models/nanosuit/nanosuit.obj")
        {
            model = glm::mat4(1.0f);
            model =  glm::translate(model, glm::vec3(3.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        }

        void Update() {

        }

        void RenderScene(learnopengl::Shader & shader) {
            shader.use();
            shader.setMat4("model", model);
            suitModel.Draw(shader);
        }
    };
}
