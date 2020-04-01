#include"LightNodeBase.hpp"

bogong::node::LightNodeBase::LightNodeBase(std::string name) 
	:
	NodeBase(name)
{

}

float bogong::node::LightNodeBase::GetIntensity()
{
	return intensity;
}

glm::vec3 bogong::node::LightNodeBase::GetColour()
{
	return colour;
}

bool bogong::node::LightNodeBase::isOn()
{
	return on;
}
