#pragma once
#include "Scene.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/NodeBase.hpp"
#include "Nodes/MaterialData.hpp"
#include "../ShaderManager.hpp"
namespace bogong {
	struct StateCache {
		glm::mat4 model;
		glm::mat4 view;
	};

	//      R
	//    /   \
	//   S1   S2
	
	class SceneRenderer {

	private:
		VertexArray vao;
		StateCache cache;
		std::stack<StateCache> state;

	public:
		void ProcessNode( std::shared_ptr<node::NodeBase> node ) {
			cache = state.top();
			cache.model = node->GetModel()*cache.model;
			state.push(cache);
			auto vn = node->GetChilds();
			for (auto & n : vn) {
				ProcessNode(n);
			}
			state.pop();
			cache = state.top();
			glm::mat4 model = cache.model;
			auto type = node->GetType();
			switch (type) {
				case node::NodeType::Shape:
				{

					auto shape_node = std::dynamic_pointer_cast<node::ShapeNode>(node);
					Configuration configuration;
					std::string name = "Colour";
					Program prog = ShaderManager::GetShader(name, configuration);
					vao.Bind();

					bufferMeshData(data); 
					 
					bindMeshData(data);
					setShaderProps(id,cache);
					// callDrawFunction();
					break;
				}
				default:
				{
					std::cout << "Unknown node type.\n";
					std::system("pause");
					break;
				}
			}
		}
		
		}

	};
}