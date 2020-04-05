#pragma once
#include "NodeBase.hpp"
#include "MaterialData.hpp"
#include "../../Texture.h"
namespace bogong {
	namespace node {

		/*
			root_node = std::make_shared<Node>("Simple Scene");

			auto cube_node = std::make_shared<ShapeNode>(cube_mesh,RED, );
			root_node->addNode(cube_node);
			



			auto  mesh =  node->getMesh();
			renderer->bindMesh(mesh);
			if( node has mtl ){
				auto mtl = node->getMtl();
				renderer->bindMtl(mtl);
			}
			if( node has Texture){
			    auto mtl = node->getTex();
				renderer->bindTex();
			}
			
		*/

		MaterialData basic_mtl = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };
		class ShapeNode : public NodeBase {
			bool isColored = false;
			bool isTextured = false;
			bool hasMaterial = false;
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Texture> tex;
			MaterialData mtl;
			glm::mat4 model;
			glm::vec3 pos;
			glm::vec3 colour;
		public:
			ShapeNode(std::string name);
			ShapeNode() = delete;
			ShapeNode(const ShapeNode & shnode);
			ShapeNode(std::shared_ptr<Mesh> & mesh,
				      glm::vec3 colour = glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3 pos = glm::vec3(0.0f),
					  std::string name = "ShapeNode")
				:
				NodeBase(name),
				mesh(mesh),
				colour(colour),
				pos(pos)
			{
				isColored = true;
			}
			ShapeNode(std::shared_ptr<Mesh> & mesh,
					  glm::vec3 colour = glm::vec3(1.0f,0.0f,0.0f),
				      MaterialData mtl = basic_mtl,
				      glm::vec3 pos = glm::vec3(0.0f),
					  std::string name = "ShapeNode")
				:
				NodeBase(name),
				mesh(mesh),
				mtl(mtl),
				colour(colour),
				pos(pos)
			{
				isColored = true;
				hasMaterial = true;
			}
			ShapeNode(std::shared_ptr<Mesh> & mesh,
				std::shared_ptr<Texture> tex,
				MaterialData mtl = basic_mtl,
				glm::vec3 pos = glm::vec3(0.0f),
				std::string name = "ShapeNode")
				:
				NodeBase(name),
				mesh(mesh),
				mtl(mtl),
				tex(tex),
				pos(pos)
			{
				hasMaterial = true;
				isTextured = true;
			}
			ShapeNode(std::shared_ptr<Mesh> & mesh,
				std::shared_ptr<Texture> tex,
				glm::vec3 pos = glm::vec3(0.0f),
				std::string name = "ShapeNode")
				:
				NodeBase(name),
				mesh(mesh),
				tex(tex),
				pos(pos)
			{
				isTextured = true;
			}
			~ShapeNode();
			void setMaterial(MaterialData mtl) {
				this->mtl = mtl;
			}
			void setPos(glm::vec3 pos) {
				this->pos = pos;
			}
			void setModel(glm::mat4 model) {
				this->model = model;
			}
			glm::vec3 getPos() const{
				return pos;
			}
			glm::mat4 getModel() const{
				return model;
			}
			std::shared_ptr<Mesh> getMesh() const{
				return mesh;
			}
			void Draw();//I guess

		};
	}
}