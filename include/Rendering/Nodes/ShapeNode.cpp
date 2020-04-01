#include "ShapeNode.hpp"

 bogong::node::ShapeNode::ShapeNode(std::string name)
	:
	NodeBase(name)
{

}

bogong::node::ShapeNode::ShapeNode(const ShapeNode & shnode)
	:
	NodeBase(shnode)
{
}

 bogong::node::ShapeNode::~ShapeNode() {}

 void bogong::node::ShapeNode::Draw() {}
