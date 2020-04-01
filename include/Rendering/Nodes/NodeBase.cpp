#include "NodeBase.hpp"

 bogong::node::NodeBase::NodeBase(const NodeBase & node) {}

 bogong::node::NodeBase::NodeBase(const std::string & name) {
	this->name = name;
}

 bogong::node::NodeBase::~NodeBase() {  }

 void bogong::node::NodeBase::AddChild(const std::shared_ptr<NodeBase> node) {
	child.push_back(node);
}

 bogong::node::vNode & bogong::node::NodeBase::GetChilds() {
	return child;
}

 bogong::node::NodeType bogong::node::NodeBase::GetType() {
	return type;
}

 std::string bogong::node::NodeBase::GetName() {
	return name;
}
