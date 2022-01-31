#pragma once
#include "Scene.hpp"
#include "../Models/Cube.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/PointLightNode.hpp"
#include "Nodes/SpotLightNode.hpp"
#include "Nodes/DirectionalLightNode.hpp"
#include "DeferredSceneManager.hpp"
#include "../Models/TextureFactory.hpp"
#include "AssimpLoader.hpp"
#include <future>
#include "../Screen.hpp"
namespace bogong {
	class SampleScene {
		std::shared_ptr<CubeMesh> cube;
		std::shared_ptr<TexturedCubeMesh> texcube;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<DefferedSceneManager> manager;
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
			manager = std::make_shared<DefferedSceneManager>();
			cam      = std::make_shared<FPCamera>();
			scene    = std::make_shared<Scene>();
			root     = std::make_shared<node::NodeBase>
				("Root",glm::vec3(0.0f,0.0f,0.0f));
			manager->SetCamera(cam);

			manager->init_scene(root);
		}
		
        void Draw() {
            manager->init_render(root);
            unsigned int out = manager->Draw(scene);
            ImGui::Begin("Output"); 
            {
                ImVec2 uv1 = ImVec2(0, 1);
                ImVec2 uv2 = ImVec2(1, 0);
                ImTextureID id = (ImTextureID)out;
                glBindTexture(GL_TEXTURE_2D, out);
                ImVec2 outputWindowSize = ImGui::GetWindowSize();
                ImGui::Image(id, outputWindowSize, uv1, uv2);
            }
            ImGui::End();
			manager->clear();
		}

		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse, 
			float delta, GLFWwindow * window){
            bool mainWindow = true;

            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            ImGui::Begin("Bogong Main Window", &mainWindow, window_flags);
            ImVec2 uv1 = ImVec2(0, 1);
            ImVec2 uv2 = ImVec2(1, 0);
            ImGuiWindowFlags output_window = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
            ImGui::End();
            {
                ImGui::Begin("Albedo Output", &mainWindow);
                ImVec2 outputWindowSize = ImGui::GetWindowSize();
                ImTextureID gAlbedoSpec = (ImTextureID)manager->gAlbedoSpec;
                ImGui::Image(gAlbedoSpec, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("Position Output", &mainWindow);
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID gPos = (ImTextureID)manager->gPos;
                glBindTexture(GL_TEXTURE_2D, manager->gPos);
                ImGui::Image(gPos, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("Normal Output", &mainWindow);
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID gNormal = (ImTextureID)manager->gNormal;
                glBindTexture(GL_TEXTURE_2D, manager->gNormal);
                ImGui::Image(gNormal, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("Depth Output");
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID gDepth = (ImTextureID)manager->gDepth;
                glBindTexture(GL_TEXTURE_2D, manager->gDepth);
                ImGui::Image(gDepth, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("SSAO Output");
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID ssaoTex = (ImTextureID)manager->ssaoTex;
                glBindTexture(GL_TEXTURE_2D, manager->ssaoTex);
                ImGui::Image(ssaoTex, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("SSR Output");
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID ssrTex = (ImTextureID)manager->ssrTex;
                glBindTexture(GL_TEXTURE_2D, manager->ssrTex);
                ImGui::Image(ssrTex, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("SSR Reflected Colors Output");
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID reflectedColorTex = (ImTextureID)manager->reflectedColorTex;
                glBindTexture(GL_TEXTURE_2D, manager->reflectedColorTex);
                ImGui::Image(reflectedColorTex, outputWindowSize, uv1, uv2);
                ImGui::End();
                ImGui::Begin("Box Blur Output");
                outputWindowSize = ImGui::GetWindowSize();
                ImTextureID boxBlurTex = (ImTextureID)manager->boxBlurTex;
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, manager->boxBlurTex);
                ImGui::Image(boxBlurTex, outputWindowSize, uv1, uv2);
                ImGui::End();
            }
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


