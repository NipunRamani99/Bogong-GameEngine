#include "CameraNode.hpp"

 void bogong::node::CameraNode::SetPos(glm::vec3 pos) {
	this->pos = pos;
}

 void bogong::node::CameraNode::setNearF(float nearF) {
	this->nearF = nearF;
}

 void bogong::node::CameraNode::setFarF(float farF) {
	this->farF = farF;
}

 void bogong::node::CameraNode::setTarget(glm::vec3 ta) {
	target = ta;
}
