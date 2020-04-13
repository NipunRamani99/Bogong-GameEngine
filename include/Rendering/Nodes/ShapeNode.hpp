#pragma once
#include "NodeBase.hpp"
#include "MaterialData.hpp"
#include "../../Texture.h"
namespace bogong {
	namespace node {
		class ShapeNode : public NodeBase {
			
			bool isColored   = false;
			bool isTextured  = false;
			bool hasMaterial = false;
			bool indexed     = false;
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Texture> tex;
			node::MaterialData mtl;
			glm::mat4 model;
			glm::vec3 pos;
			glm::vec3 colour;
		public:
			
			ShapeNode() = delete;
			ShapeNode(const ShapeNode & shnode) 
				:
				NodeBase(shnode)
			{
				type   = NodeType::Shape;
				mesh   = shnode.mesh;
				tex    = shnode.tex;
				mtl    = shnode.mtl;
				model  = shnode.model;
				pos    = shnode.pos;
				colour = shnode.colour;
				model  = glm::translate(pos);
				type   = node::NodeType::Shape;
			}
			/*
				ShapeNode(std::string mesh_name){
				   
				}
			*/


			ShapeNode(std::shared_ptr<Mesh>  mesh,
				      glm::vec3 colour = glm::vec3(1.0f, 0.0f, 0.0f),
					  glm::vec3 pos    = glm::vec3(0.0f),
				      std::string name = "ShapeNode");
			ShapeNode(std::shared_ptr<Mesh>  mesh,
					  node::MaterialData mtl,
				      glm::vec3 colour = glm::vec3(1.0f,0.0f,0.0f),
				      glm::vec3 pos    = glm::vec3(0.0f),
					  std::string name = "ShapeNode")
				:
				NodeBase(name,pos),
				mesh(mesh),
				mtl(mtl),
				colour(colour),
				pos(pos)
			{
				isColored   = true;
				hasMaterial = true;
				type        = node::NodeType::Shape;
			}
			ShapeNode(std::shared_ptr<Mesh> mesh,
				std::shared_ptr<Texture> tex,
				node::MaterialData mtl,
				glm::vec3 pos    = glm::vec3(0.0f),
				std::string name = "ShapeNode");
			ShapeNode(std::shared_ptr<Mesh> mesh,
				std::shared_ptr<Texture> tex,
				glm::vec3 pos    = glm::vec3(0.0f),
				std::string name = "ShapeNode");
			~ShapeNode()
			{
			}

			void setMaterial(MaterialData mtl);
			void setPos(glm::vec3 pos);
			void setModel(glm::mat4 model);
			bool isIndexed() { return indexed; }
			glm::vec3 getPos() const;
			glm::mat4 getModel() const;
			glm::vec3 getColour() { return colour; }
			std::shared_ptr<Mesh> getMesh() const;
			std::shared_ptr<Texture> getTexture(){ return tex; }
			bool hasTexture() { return isTextured; }
		};
	}
}