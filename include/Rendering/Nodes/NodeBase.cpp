#include "NodeBase.hpp"

 bogong::node::NodeBase::NodeBase(const NodeBase & node) 
	 :
	 name(node.name)
 {
 }

 bogong::node::NodeBase::NodeBase(const std::string & name) {
	this->name = name;
}

 bogong::node::NodeBase::NodeBase(const std::string name, const glm::vec3 pos)
 {
	 this->name = name;
	 model = glm::translate(glm::mat4(1.0f),pos);
 }
 
 bogong::node::NodeBase::~NodeBase() {  }
 void bogong::node::NodeBase::UpdateTree() {
	 if (!dirty) { return; }
	 dirty = false;
	 absTrans = model;
	 if (!parent) {
		 relTrans = absTrans;
	 }
	 else
	 {
		 relTrans = parent->relTrans * absTrans;
	 }
	 for (auto & n : child) {
		 n->UpdateTree();
	 }
 }
 void bogong::node::NodeBase::markdirty() {
	 if (dirty)return;
	 dirty = true;
	 
	 for (auto ch : child) {
		 ch->markdirty();
	 }
	 auto p = parent;
	 while (p) {
		 p->markdirty();
		 p = p->parent;
	 }
 }
 void bogong::node::NodeBase::AddChild(const std::shared_ptr<NodeBase> node) {
	node->parent = this;
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
