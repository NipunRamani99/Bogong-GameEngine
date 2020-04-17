#include "Scene.hpp"
#include <vector>
#include "Nodes/ShapeNode.hpp"
bogong::Scene::Scene() {}

 void bogong::Scene::SetRootNode(std::shared_ptr<node::NodeBase> node) {
	root_node = node;

}
