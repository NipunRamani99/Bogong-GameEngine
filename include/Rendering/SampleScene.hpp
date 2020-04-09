#pragma once
#include "Scene.hpp"
#include "../Models/Cube.hpp"
#include "Nodes/ShapeNode.hpp"
#include "SceneRenderer.hpp"
namespace bogong {
	class SampleScene {
		std::shared_ptr<CubeMesh> cube;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<SceneRenderer> renderer;
		std::shared_ptr<FPCamera> cam;
	public:
		SampleScene() {
			renderer = std::make_shared<SceneRenderer>();
			cam = std::make_shared<FPCamera>();
			scene = std::make_shared<Scene>();
			cube = std::make_shared<CubeMesh>(glm::vec4(1.0f));
			std::shared_ptr<node::NodeBase> root = std::make_shared<node::NodeBase>("Root",glm::vec3(0.0f,0.0f,0.0f));
			std::shared_ptr<node::ShapeNode> snode = std::make_shared<node::ShapeNode>(std::dynamic_pointer_cast<Mesh>(cube), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), "Cube1");
			std::shared_ptr<node::ShapeNode> snode2 = std::make_shared<node::ShapeNode>(std::dynamic_pointer_cast<Mesh>(cube), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), "Cube2");

			root->AddChild(snode);
			root->AddChild(snode2);
			scene->SetRootNode(root);
			scene->SetCamera(cam);
		}
		void Draw() {
 			renderer->Draw(scene);
		}
		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse, float delta) {
			scene->Update(kbd, mouse, delta);
		}
	};
}


