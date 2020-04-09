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

		class ShapeNode : public NodeBase {
			
			bool isColored = false;
			bool isTextured = false;
			bool hasMaterial = false;
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Texture> tex;
			node::MaterialData mtl;
			glm::mat4 model;
			glm::vec3 pos;
			glm::vec3 colour;
		public:
			ShapeNode(std::string name) 
				:
				NodeBase(name)
			{
				type = NodeType::Shape;
			}
			ShapeNode() = delete;
			ShapeNode(const ShapeNode & shnode) 
				:
				NodeBase(shnode)
			{
				type = NodeType::Shape;
			}
			/*
				ShapeNode(std::string mesh_name){
				   
				}
			*/


			ShapeNode(std::shared_ptr<Mesh> & mesh,
				      glm::vec3 colour = glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3 pos = glm::vec3(0.0f),
				std::string name = "ShapeNode");
			ShapeNode(std::shared_ptr<Mesh> & mesh,
					  node::MaterialData mtl,
				      glm::vec3 colour = glm::vec3(1.0f,0.0f,0.0f),
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
				node::MaterialData mtl,
				glm::vec3 pos = glm::vec3(0.0f),
				std::string name = "ShapeNode");
			ShapeNode(std::shared_ptr<Mesh> & mesh,
				std::shared_ptr<Texture> tex,
				glm::vec3 pos = glm::vec3(0.0f),
				std::string name = "ShapeNode");
			~ShapeNode()
			{
			}
			void setMaterial(MaterialData mtl);
			void setPos(glm::vec3 pos);
			void setModel(glm::mat4 model);
			glm::vec3 getPos() const;
			glm::mat4 getModel() const;
			std::shared_ptr<Mesh> getMesh() const;

		};
	}
}