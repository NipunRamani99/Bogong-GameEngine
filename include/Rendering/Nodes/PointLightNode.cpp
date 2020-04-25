#include "PointLightNode.hpp"

 bogong::node::PointLightNode::PointLightNode(PointLight pl, std::string name)
	:
	LightNodeBase(name,pl.pos)
{
	this->pl = pl;
}


 glm::vec3 bogong::node::PointLightNode::GetPos() {
	return pl.pos;
}
