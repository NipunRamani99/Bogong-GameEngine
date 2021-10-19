#pragma once
#include "Scene.hpp"
#include "../Models/Cube.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/PointLightNode.hpp"
#include "Nodes/SpotLightNode.hpp"
#include "Nodes/DirectionalLightNode.hpp"
#include "PointShadowSponzaScene.hpp"
#include "../Models/TextureFactory.hpp"
#include "AssimpLoader.hpp"
#include <future>
#include "../Screen.hpp"
namespace bogong {
	class SampleScene {
		std::shared_ptr<CubeMesh> cube;
		std::shared_ptr<TexturedCubeMesh> texcube;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<PointShadowSponzaScene> manager;
		std::shared_ptr<FPCamera> cam;
		std::shared_ptr<node::NodeBase> root;
		glm::vec3 angle = glm::vec3(0.0f,0.0f,0.0f);
		glm::vec3 alpha = glm::vec3(2.0f,5.0f,0.0f);
		glm::vec3 pos = glm::vec3(0.0f, 1.0f, 0.f);
        bool canToggle = true;
        float timer = 0.0f;
        bool isMouseEnabled = false;
	public:
	
		SampleScene() {
			manager = std::make_shared<PointShadowSponzaScene>();
			cam      = std::make_shared<FPCamera>();
			scene    = std::make_shared<Scene>();
			root     = std::make_shared<node::NodeBase>
				("Root",glm::vec3(0.0f,0.0f,0.0f));
			manager->SetCamera(cam);

			manager->init_scene(root);
		}
		
        void Draw() {
			manager->init_render(root);
 			manager->Draw(scene);
			manager->clear();
		}

		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse, 
			float delta, GLFWwindow * window){
            toggle(kbd, mouse, delta, window);
			angle += delta * alpha*2.0f;
			manager->Update(delta);
		}

        void toggle(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse, float delta, GLFWwindow * window) {
            if (kbd->isKeyPressed(KEY::KEY_K))
            {
                if (canToggle) {
                    isMouseEnabled = !isMouseEnabled;
                    canToggle = false;
                    if (!isMouseEnabled) {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        glfwSetCursorPos(window, 640, 360);
                        mouse->x = 640;
                        mouse->y = 360;
                        cam->ClearMouse(mouse);
                        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
                    }
                    else {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

                        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
                    }
                }

            }
            if (!isMouseEnabled)
                cam->Update(kbd, mouse, delta);
            if (!canToggle) {
                timer += delta;
                if (timer >= 0.1f) {
                    canToggle = true;
                    timer = 0.0f;
                }
            }

        }
	};
}


