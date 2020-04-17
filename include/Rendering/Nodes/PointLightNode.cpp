#include "PointLightNode.hpp"

 bogong::node::PointLightNode::PointLightNode(PointLight pl, std::string name)
	:
	NodeBase(name,pl.pos)
{
	this->pl = pl;
}

 bogong::node::PointLight & bogong::node::PointLightNode::getData() {
	return pl;
}

 glm::vec3 bogong::node::PointLightNode::GetPos() {
	return pl.pos;
}
