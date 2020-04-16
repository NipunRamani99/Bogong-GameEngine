#include "ShapeNode.hpp"

bogong::node::ShapeNode::ShapeNode(std::vector<std::shared_ptr<Mesh>> &meshes, glm::vec3 colour, glm::vec3 pos, std::string name)
	:
	NodeBase(name,pos),
	colour(colour)
{

	this->meshes = meshes;
	for (auto mesh : meshes) {
		if (mesh->GetIndexBuffer()) {
			indexed = true;
		}
	}
	type      = node::Shape;
	this->pos = pos;
	isColored = true;
}

bogong::node::ShapeNode::ShapeNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> tex, glm::vec3 pos, std::string name)
	:
	NodeBase(name,pos),
	tex(tex)
{
	if (mesh->GetIndexBuffer()) {
		indexed = true;
	}

	meshes.push_back(mesh);
	type = node::Shape;
	this->pos = pos;
	hasMaterial = true;
	isTextured = true;
}
//
//bogong::node::ShapeNode::ShapeNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> tex, glm::vec3 pos, std::string name)
//	:
//	NodeBase(name,pos),
//	tex(tex)
//{
//	meshes.push_back(mesh);
//	if (mesh->GetIndexBuffer()) {
//		indexed = true;
//	}
//	type       = node::Shape;
//	this->pos  = pos;
//	isTextured = true;
//}


void bogong::node::ShapeNode::setPos(glm::vec3 pos) {
	this->pos = pos;
	glm::translate(glm::mat4(1.0f), pos);
}

void bogong::node::ShapeNode::setModel(glm::mat4 model) {
	this->model = model;
}

glm::vec3 bogong::node::ShapeNode::getPos() const {
	
	return pos;
}

glm::mat4 bogong::node::ShapeNode::getModel() const {
	return model;
}

std::vector<std::shared_ptr<bogong::Mesh>> bogong::node::ShapeNode::getMesh() const {
	return meshes;
}
