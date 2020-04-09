#include "ShapeNode.hpp"

bogong::node::ShapeNode::ShapeNode(std::shared_ptr<Mesh> mesh, glm::vec3 colour, glm::vec3 pos, std::string name)
	:
	NodeBase(name,pos),
	mesh(mesh),
	colour(colour),
	pos(pos)
{
	type = node::Shape;
	this->pos = pos;
	isColored = true;
}

bogong::node::ShapeNode::ShapeNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> tex, MaterialData mtl, glm::vec3 pos, std::string name)
	:
	NodeBase(name,pos),
	mesh(mesh),
	mtl(mtl),
	tex(tex),
	pos(pos)
{
	type = node::Shape;
	this->pos = pos;
	hasMaterial = true;
	isTextured = true;
}

bogong::node::ShapeNode::ShapeNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> tex, glm::vec3 pos, std::string name)
	:
	NodeBase(name,pos),
	mesh(mesh),
	tex(tex),
	pos(pos)
{
	type = node::Shape;
	this->pos = pos;
	isTextured = true;
}

void bogong::node::ShapeNode::setMaterial(MaterialData mtl) {
	this->mtl = mtl;
}

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

std::shared_ptr<bogong::Mesh> bogong::node::ShapeNode::getMesh() const {
	return mesh;
}
