#pragma once
#include "Scene.hpp"
#include "../Models/Cube.hpp"
#include "Nodes/ShapeNode.hpp"
#include "SceneRenderer.hpp"
#include "../Models/TextureFactory.hpp"
#include "AssimpLoader.hpp"
namespace bogong {
	class SampleScene {
		std::shared_ptr<CubeMesh> cube;
		std::shared_ptr<TexturedCubeMesh> texcube;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<SceneRenderer> renderer;
		std::shared_ptr<FPCamera> cam;
		std::shared_ptr<node::NodeBase> root;
		glm::vec3 angle = glm::vec3(0.0f,0.0f,0.0f);
		glm::vec3 alpha = glm::vec3(2.0f,5.0f,0.0f);
		glm::vec3 pos = glm::vec3(0.0f, 1.0f, 0.f);
	public:
		SampleScene() {
			renderer = std::make_shared<SceneRenderer>();
			cam = std::make_shared<FPCamera>();
			scene = std::make_shared<Scene>();
			cube = std::make_shared<CubeMesh>(glm::vec4(1.0f));
			texcube = std::make_shared<TexturedCubeMesh>();
			//auto tex1 = TextureManager::make_texture("assets/models/crate/container2.png");
			root = std::make_shared<node::NodeBase>("Root",glm::vec3(0.0f,0.0f,0.0f));
		//	auto model3 = AssimpFactory::LoadModel("C:\\Users\\Laptop.000\\Downloads\\Aircraft 3D Models with Textures\\mikoyan-mig-35-foxhound_p3dm.ru\\Mikoyan MIG-35 Foxhound\\mig35.obj");
			auto model1 = AssimpFactory::LoadModel("assets\\models\\sponza\\sponza.obj");
			//model1->Scale(glm::vec3(0.1, 0.1, 0.1));
			model1->Scale(glm::vec3(0.01, 0.01, 0.01));
			model1->Translate(glm::vec3(0.0f, 10.0f, 0.0f));
			
			//model1->AddChild(model3);
			root->AddChild(model1);
			
			scene->SetRootNode(root);
			scene->SetCamera(cam);
		}
		void Draw() {
 			renderer->Draw(scene);
		}
		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse, float delta, GLFWwindow * window){
			scene->Update(kbd, mouse, delta,window);
			angle += delta * alpha*2.0f;
			ImGui::Begin("Sample Scene");
			renderer->Update();
			ImGui::End();
			if(ImGui::InputFloat3("Pos", &pos[0], 4)) {
				root->Translate(pos);
			}
		}
	};
}


