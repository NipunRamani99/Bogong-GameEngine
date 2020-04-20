#pragma once
#include <iostream>
#include "Nodes/ShapeNode.hpp"
#include <queue>
namespace bogong {

	struct RenderQueueItem {
		std::shared_ptr<node::ShapeNode> node;
		int priority = 1;
	};

	class Renderer {
		

	public:

	};
}