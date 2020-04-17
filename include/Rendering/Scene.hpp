#pragma once
#include"Nodes/NodeBase.hpp"
#include "../Camera.h"
#include <stack>
#include "../Keyboard.h"
#include "../Mouse.h"
namespace bogong {
	class Scene {

		std::shared_ptr<node::NodeBase> root_node;
		std::shared_ptr<FPCamera> cam;
	public:
		Scene();
		void SetRootNode(std::shared_ptr<node::NodeBase> node);
		void SetCamera(std::shared_ptr<FPCamera> cam) {
			this->cam = cam;
		}
		std::shared_ptr<FPCamera> getCamera() {
			return cam;
		}
		glm::mat4 getProjection() {
			return cam->GetProjection();
		}
		glm::mat4 getView()
		{
			return cam->GetView();
		}
		std::shared_ptr<node::NodeBase> getRootNode() {
			return root_node;
		}
		void Update(std::shared_ptr<Keyboard> kbd, std::shared_ptr<Mouse> mouse,float delta){
			cam->Update(kbd, mouse, delta);
		}
	};
}