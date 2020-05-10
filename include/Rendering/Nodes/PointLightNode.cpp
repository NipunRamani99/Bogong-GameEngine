#include "PointLightNode.hpp"

 bogong::node::PointLightNode::PointLightNode(PointLight pl, std::string name)
	:
	LightNodeBase(name,pl.pos)
{
	this->pl = pl;
}


 