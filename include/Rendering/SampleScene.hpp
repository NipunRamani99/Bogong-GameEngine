#pragma once
#include "Scene.hpp"
#include "../Models/Cube.hpp"
#include "Nodes/ShapeNode.hpp"
#include "SceneRenderer.hpp"
#include "../Models/TextureFactory.hpp"
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
	public:
		SampleScene() {
			renderer = std::make_shared<SceneRenderer>();
			cam = std::make_shared<FPCamera>();
			scene = std::make_shared<Scene>();
			cube = std::make_shared<CubeMesh>(glm::vec4(1.0f));
			texcube = std::make_shared<TexturedCubeMesh>();
			auto tex1 = TextureManager::make_texture("assets/models/crate/container2.png");
			root = std::make_shared<node::NodeBase>("Root",glm::vec3(0.0f,0.0f,0.0f));
			std::shared_ptr<node::ShapeNode> snode = std::make_shared<node::ShapeNode>(std::dynamic_pointer_cast<Mesh>(cube), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.50f, 0.0f, 0.0f), "Cube1");
			std::shared_ptr<node::ShapeNode> snode2 = std::make_shared<node::ShapeNode>(std::dynamic_pointer_cast<Mesh>(cube), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.50f, 0.0f, 0.0f), "Cube2");
			
			std::shared_ptr <node::ShapeNode> snode3 = std::make_shared<node::ShapeNode>(std::dynamic_pointer_cast<Mesh>(cube),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),"Cube3" );
			std::shared_ptr<node::ShapeNode> snode4 = std::make_shared<node::ShapeNode>(std::dynamic_pointer_cast<Mesh>(texcube), tex1, glm::vec3(3.0f, 3.0f, 0.0f), "TexCube");
			root->AddChild(snode);
			root->AddChild(snode2);
			root->AddChild(snode4);
			snode2->AddChild(snode3);
			scene->SetRootNode(root);
			scene->SetCamera(cam);
		}
		void Draw() {
 			renderer->Draw(scene);
		}
		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse, float delta) {
			scene->Update(kbd, mouse, delta);
			angle += delta * alpha*2.0f;
			root->Rotate(angle);
		}
	};
}


