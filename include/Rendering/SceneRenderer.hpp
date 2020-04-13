#pragma once
#include "Scene.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/NodeBase.hpp"
#include "Nodes/MaterialData.hpp"
#include "../ShaderManager.hpp"
namespace bogong {
	struct StateCache {
		glm::mat4 model;

	};

	//      R
	//    /   \
	//   S1   S2
	//   /\   /\

	class SceneRenderer {

	private:
		VertexArray vao;
		std::stack<StateCache> state;
		std::shared_ptr<FPCamera> cam;
		glm::mat4 view;
		glm::mat4 projection;

		bool attribEnabled = false;
	public:
	
		void DrawMesh(std::shared_ptr<node::ShapeNode> sn,StateCache cach) {
			Configuration configuration;
			bool tex = sn->hasTexture();
			if (tex) {
				configuration.macros.push_back("HAS_UV");
			}
			std::string name = "Colour";
			Program prog = ShaderManager::GetShader(name, configuration);
			prog.Bind();
			auto mesh = sn->getMesh();
			auto buffer = mesh->GetBuffer();
			buffer[0].first->Bind();
			int stride = tex ? sizeof(float) * 8 : sizeof(float) * 6;
			CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0));
			CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 3)));
			if (tex) {

				CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float)*6)));

			}
			if (!tex) {
				prog.setVec4("object_colour", glm::vec4(sn->getColour(),1.0f));
			}
			else
			{
				sn->getTexture()->Bind();
				prog.setInt("s", 0);
			}
			
			auto model = cach.model * sn->GetModel();
			prog.setMat4("model", model);
			prog.setMat4("view", view);
			prog.setMat4("projection", projection);
			unsigned int count = mesh->GetCount();

			CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, count));
		}
		void ProcessNode(std::shared_ptr<node::NodeBase> node) {
 			StateCache cache = state.top();
			cache.model = cache.model * node->GetModel();
			state.push(cache);
			auto vn = node->GetChilds();
			for (auto & n : vn) {
				ProcessNode(n);
			}
			state.pop();
			cache = state.top();
			auto type = node->GetType();
			switch (type) {
			case node::NodeType::Shape:
			{

				auto shape_node = std::dynamic_pointer_cast<node::ShapeNode>(node);
				DrawMesh(shape_node,cache);
				break;
			}
			case node::NodeType::Root:
			{
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
		void Draw(std::shared_ptr<Scene> scene) {
			auto root = scene->getRootNode();
			glm::mat4 model = root->GetModel();
			StateCache cache;
			auto cam = scene->getCamera();
			projection = cam->GetProjection();
			view = cam->GetView();
			cache.model = glm::mat4(1.0f);
			state.push(cache);
			vao.Bind();
			CHECK_GL_ERROR(glEnableVertexArrayAttrib(vao.GetID(), 0));
			CHECK_GL_ERROR(glEnableVertexArrayAttrib(vao.GetID(), 1));
			CHECK_GL_ERROR(glEnableVertexArrayAttrib(vao.GetID(), 2));

			ProcessNode(root);
		}
	};

}
