#pragma once
#include "../Mesh.hpp"
#include "../Renderer.hpp"
#include "../../Entity.hpp"
#include <glm/glm.hpp>
#include<vector>
#include<memory>
namespace bogong {
	namespace node {
		enum NodeType {
			Root,
			Group,
			Material,
			Shape,
			Light
		};
		class NodeBase;
		typedef std::vector<std::shared_ptr<NodeBase>> vNode;
		class NodeBase : public Entity{
		protected:
			vNode child;
			std::string name = "";
			NodeBase * parent = nullptr;
			bool dirty = false;
		public:
			glm::mat4 relTrans;
			glm::mat4 absTrans;
			NodeType type = Root;

			NodeBase() = delete;
			NodeBase(const NodeBase & node);
			NodeBase(const std::string & name);
			NodeBase(const std::string name, const glm::vec3 pos = glm::vec3(1.0f));
			virtual ~NodeBase();
			void AddChild(const std::shared_ptr<NodeBase> node);
			vNode & GetChilds();
			NodeType GetType();
			void UpdateTree();
			void UpdateModel() {
				model = glm::mat4(1.0f);
				model = glm::scale(model, scale);
				model = glm::translate(model, pos);
				model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			void markdirty();
			glm::mat4 GetModel() { 
				return model; 
			}
			std::string GetName();
		};
	}
	
}