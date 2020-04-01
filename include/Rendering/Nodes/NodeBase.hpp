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
			vNode child;
			std::string name = "";
			NodeType type;
		public:
			NodeBase() = delete;
			NodeBase(const NodeBase & node);
			NodeBase(const std::string & name);
			~NodeBase();
			void AddChild(const std::shared_ptr<NodeBase>  node);
			vNode & GetChilds();
			NodeType GetType();
			std::string GetName();
		};
	}
	
}