#pragma once
#include"Nodes/NodeBase.hpp"
#include "../Camera.h"
#include <stack>
#include "../Keyboard.h"
#include "../Mouse.h"
namespace bogong {
	class Scene {

		std::shared_ptr<node::NodeBase> root_node;
	public:
		Scene();
		void SetRootNode(std::shared_ptr<node::NodeBase> node);
		void SetCamera() {}
		void Update() {}
		void Draw(Shader & shader);
	};
}