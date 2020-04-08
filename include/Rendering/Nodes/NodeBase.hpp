#pragma once
#include "../Mesh.hpp"
#include "../Renderer.hpp"
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
		class NodeBase {
		protected:
			vNode child;
			std::string name = "";
			NodeType type;
			std::shared_ptr<NodeBase> parent;
			glm::mat4 model;
		public:
			NodeBase() = delete;
			NodeBase(const NodeBase & node);
			NodeBase(const std::string & name);
			virtual ~NodeBase();
			void AddChild(const std::shared_ptr<NodeBase> node);
			vNode & GetChilds();
			NodeType GetType();
			void applyTransform(glm::mat4 p_model) {
				model = p_model * model;
			}
			glm::mat4 GetModel() { return model; }
			std::string GetName();
		};
	}
	
}