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
			NodeType type = Root;
			std::shared_ptr<NodeBase> parent;
		public:
			NodeBase() = delete;
			NodeBase(const NodeBase & node);
			NodeBase(const std::string & name);
			NodeBase(const std::string name, const glm::vec3 pos = glm::vec3(1.0f));
			virtual ~NodeBase();
			void AddChild(const std::shared_ptr<NodeBase> node);
			vNode & GetChilds();
			NodeType GetType();
			
			glm::mat4 GetModel() { return model; }
			std::string GetName();
		};
	}
	
}