#include "PointLightNode.hpp"

bogong::node::PointLightNode::PointLightNode(std::string name)
	:
	LightNodeBase(name)
{
}

glm::vec3 bogong::node::PointLightNode::GetPos()
{
	return pos;
}
