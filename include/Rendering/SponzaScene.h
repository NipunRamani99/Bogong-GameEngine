#pragma once
#include "../LearnOpenGL/Model.h"
namespace bogong {
    class SponzaScene {
    private:
        learnopengl::Model sponzaModel;
        glm::mat4 model;
    public:
        SponzaScene() 
            :
            sponzaModel("assets/models/Sponza/sponza.obj")
        {
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
          
        }

        void Update() {

        }

        void RenderScene(learnopengl::Shader & shader) {
            shader.setMat4("model", model);
            sponzaModel.Draw(shader);
        }
    };
}
