#pragma once
#include "Scene.hpp"
#include "../Models/Cube.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/PointLightNode.hpp"
#include "Nodes/SpotLightNode.hpp"
#include "Nodes/DirectionalLightNode.hpp"
#include "SceneRenderer.hpp"
#include "../Models/TextureFactory.hpp"
#include "AssimpLoader.hpp"
#include <future>
#include "../Screen.hpp"
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
			cam      = std::make_shared<FPCamera>();
			scene    = std::make_shared<Scene>();
			root     = std::make_shared<node::NodeBase>
				("Root",glm::vec3(0.0f,0.0f,0.0f));
			auto model1 = AssimpFactory::LoadModel("assets\\models\\nanosuit\\nanosuit.obj");
			auto sl = node::SpotLight();
			sl.ambient = glm::vec3(0.6f, 0.6f, 0.6f);
			sl.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
			sl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
			sl.dir = glm::vec3(0.0f, 0.0f, -1.0f);
			sl.constant = 1.0f;
			sl.pos = glm::vec3(0.0f, 1.50f, 1.00f);
			sl.innercutoff = 0.91;
			sl.outercutoff = 0.82;
			sl.linear = 0.0f;
			sl.quadratic = 0.0f;
			auto dl = node::DirectionalLight();
			dl.diffuse = glm::vec3(1.0f,0.0f,0.0f);
			dl.ambient = glm::vec3(0.2, 0.2, 0.2);
			dl.specular = glm::vec3(0.0, 0.5, 0.0);
			dl.dir = glm::vec3(-1.0, 0.0, 0.0);
			auto spotlight = std::make_shared<node::SpotLightNode>(sl, glm::vec3(0., 0., 0.));
			auto dirlight = std::make_shared<node::DirectionalLightNode>(dl, glm::vec3(0.0,0.0, 0.));
			model1->AddChild(spotlight);
			model1->AddChild(dirlight);
			model1->Scale(glm::vec3(0.1, 0.1, 0.1));
			model1->Translate(glm::vec3(0.0f, 4.0f, 0.0f));
			root->AddChild(model1);
			root->UpdateTree();
			scene->SetRootNode(root);
			scene->SetCamera(cam);
			renderer->SetCamera(cam);


		}
		void Draw() {
			renderer->init(root);
 			renderer->Draw(scene);
			renderer->clear();
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


