#pragma once
#include "Scene.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/NodeBase.hpp"
#include "Nodes/MaterialData.hpp"
#include "Nodes/DirectionalLightNode.hpp"
#include "Nodes/PointLightNode.hpp"
#include "Nodes/SpotLightNode.hpp"
#include "../ShaderManager.hpp"
#include "Framebuffer.hpp"
#include "../Screen.hpp"
#include "Rendering.hpp"
namespace bogong {
	
	class SceneManager {

	private:
		VertexArray vao2;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<FPCamera> cam;
		std::shared_ptr<Screen> scr;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 light_pos      = glm::vec3(4.0f, 5.0f, 6.0f);
		glm::vec3 light_ambient  = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 light_diffuse  = glm::vec3(0.8f, 0.8f, 0.8f);
		glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		Program output;
		Program test;
		
		std::shared_ptr<Framebuffer> frame;
		float quadVertices[24] = {  -1.0f,  1.0f,  0.0f, 1.0f,
									-1.0f, -1.0f,  0.0f, 0.0f,
									 1.0f, -1.0f,  1.0f, 0.0f,
									-1.0f,  1.0f,  0.0f, 1.0f,
									 1.0f, -1.0f,  1.0f, 0.0f,
									 1.0f,  1.0f,  1.0f, 1.0f};
		bool attribEnabled = false;
		std::shared_ptr<RendererDude> dude;
		std::vector<node::NodeBase*> vecNodes;
		int node_idx = 0;
		int selected_node = -1;
		bool open = false;
	public:
		SceneManager()
		{
			scr = std::make_shared<Screen>();
			vbo = std::make_shared<VertexBuffer>(quadVertices, sizeof(float) * 24);
			//frame = std::make_shared<Framebuffer>(1280,640);
			//frame->InitDepthStencilAndColour();
			Configuration config;
			output = ShaderManager::GetShader("Frame", config);
			config.macros.push_back("HAS_UV");
			config.macros.push_back("MATERIAL_WITH_TEX");
			test = ShaderManager::GetShader("MultipleLight", config);
			dude = std::make_shared<RendererDude>();
		}
		void UpdateLight(node::NodeBase * n) {
			node::LightNodeBase* ln = (node::LightNodeBase*)(n);
			switch (ln->light_type) {
			case node::Point: { 
				node::PointLightNode * pn = (node::PointLightNode*)(ln);
				ImGui::InputFloat3("Ambient: ", &pn->pl.ambient[0], 4);
				ImGui::InputFloat3("Diffuse: ", &pn->pl.diffuse[0], 4);
				ImGui::InputFloat3("Specular: ", &pn->pl.specular[0], 4);
				ImGui::InputFloat3("Position: ", &pn->pl.pos[0], 4);

				break;
			}
			case node::Directional: {
				node::DirectionalLightNode * dn = (node::DirectionalLightNode*)(ln);
				ImGui::InputFloat3("Ambient: ", &dn->dl.ambient[0], 4);
				ImGui::InputFloat3("Diffuse: ", &dn->dl.diffuse[0], 4);
				ImGui::InputFloat3("Specular: ", &dn->dl.specular[0], 4);
				ImGui::InputFloat3("Direction: ", &dn->dl.dir[0], 4);

				break;
			}
			case node::Spot: {
				node::SpotLightNode * sl  = (node::SpotLightNode*)(ln);
				ImGui::InputFloat3("Ambient: ",   &sl->sl.ambient[0], 4);
				ImGui::InputFloat3("Diffuse: ",   &sl->sl.diffuse[0], 4);
				ImGui::InputFloat3("Specular: ",  &sl->sl.specular[0], 4);
				ImGui::InputFloat3("Direction: ", &sl->sl.dir[0], 4);
				ImGui::InputFloat3("Position: ",  &sl->sl.pos[0], 4);
				break;
			}

			}
		}
		void UpdateModel(node::NodeBase * n) {
			
			if (ImGui::InputFloat3("Position: ", &n->pos[0], 4)) {
				n->markdirty();
				n->UpdateModel();
			}
			if (ImGui::InputFloat3("Scale: ", &n->scale[0], 4)) {
				n->markdirty();
				n->UpdateModel();
			}

		}
		void Update() {
			
			scr->Update(cam->GetPos(), cam->GetDir(), cam->GetView(),cam->GetProjection());
			if (selected_node != -1) {
				ImGui::Begin("Control");
				{
					node::NodeBase * n = vecNodes[selected_node];
					ImGui::Text(n->GetName().c_str());
					if (n->GetType() == node::Light) {
						UpdateLight(n);
					}
					if (n->GetType() == node::Shape) {
						UpdateModel(n);
					}
					if (n->GetType() == node::Root) {
						UpdateModel(n);
					}
				}
				ImGui::End();
			}
			vecNodes[0]->UpdateTree();
		}
		void clear() {
			dude->clear();
		}
		void init_scene(std::shared_ptr<node::NodeBase> node) {
			node::NodeBase * n = &*node;
			std::queue<node::NodeBase*> q;
			q.push(n);
			while (!q.empty()) {
				n = q.front();
				q.pop();
				vecNodes.push_back(n);
				for (auto ch : n->GetChilds()) {
					q.push(&*ch);
				}
			}
		}
		void init_render(std::shared_ptr<node::NodeBase> node) {
			node_idx = 0;
			ImGui::Begin("Scene");
			open = ImGui::TreeNode("Sample Scene");
			ProcessNode(node,open);
			if(open)  ImGui::TreePop();
			ImGui::End();
			dude->BindLights(test);

		}
		void ProcessNode(std::shared_ptr<node::NodeBase> node, bool enabled) {
			bool opened = false;
			if (node) {
				if (enabled) {
					const auto flag = (node->GetChilds().size()==0?ImGuiTreeNodeFlags_Leaf:0)| (selected_node == node_idx ? ImGuiTreeNodeFlags_Selected : 0);
					 opened = ImGui::TreeNodeEx((void*)(intptr_t)node_idx, flag, node->GetName().c_str());
					if (ImGui::IsItemClicked()) {
						selected_node = node_idx;
					}
				}
				auto vn = node->GetChilds();
				for (auto & n : vn) {
					node_idx++;
					ProcessNode(n,opened);
				}
				if (opened)ImGui::TreePop();
				auto type = node->GetType();
				switch (type) {
				case node::NodeType::Shape:
				{

					auto shape_node = std::dynamic_pointer_cast<node::ShapeNode>(node);
					for (auto m : shape_node->getMesh()) {
						RenderQueueItem item;
						if (m->getTexMaterial()->diffuse)
							item.priority = m->getTexMaterial()->diffuse->m_TexID;
						item.node = &*m;
						item.trans = shape_node->relTrans;
						
						dude->AddShapeNode(item);
					}
					break;
				}
				case node::NodeType::Light:
				{
					auto light_node = std::dynamic_pointer_cast<node::LightNodeBase>(node);
					dude->AddLight(&*light_node);
					break;
				}
				case node::NodeType::Root:
				{
					break;
				}
				default:
				{
					break;
				}
				}
			}
		}
		void DrawFrame() {
			vao2.Bind();
			vbo->Bind();
			output.Bind();
			frame->GetColourTexture()->Bind();
			output.setInt("s", 0);
			int stride = sizeof(float) * 4;

			CHECK_GL_ERROR(glEnableVertexArrayAttrib(vao2.GetID(), 0));
			CHECK_GL_ERROR(glEnableVertexArrayAttrib(vao2.GetID(), 1));
			CHECK_GL_ERROR(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0));
			CHECK_GL_ERROR(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 2)));

			CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
		}
		void SetCamera(std::shared_ptr<FPCamera>  cam) {
			this->cam = cam;
		}
		void Draw(std::shared_ptr<Scene> scene) {
			scr->Draw(0.2f);
		//	dude->prerender();
			dude->Render(test,cam->GetPos() ,cam->GetProjection(), cam->GetView());
		}
	};

}
