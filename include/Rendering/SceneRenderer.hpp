#pragma once
#include "Scene.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Nodes/NodeBase.hpp"
#include "Nodes/MaterialData.hpp"
#include "../ShaderManager.hpp"
#include "Framebuffer.hpp"
#include "../Screen.hpp"
#include "Rendering.hpp"
namespace bogong {
	struct StateCache {
		glm::mat4 model;
		
	};
	class SceneRenderer {

	private:
		VertexArray vao;
		VertexArray vao2;
		std::shared_ptr<VertexBuffer> vbo;
		std::stack<StateCache> state;
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
	public:
		SceneRenderer()
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
		
		void DrawMesh(std::shared_ptr<node::ShapeNode> sn) {
			Configuration configuration;
			auto meshes = sn->getMesh();
			std::string nodename = sn->GetName();
			for(auto mesh : meshes)
			{
				bool tex = mesh->isTextured();

				if (tex) {
					configuration.macros.push_back("HAS_UV");
					configuration.macros.push_back("MATERIAL_WITH_TEX");
				}
				std::string name = "Phong";
				Program prog = ShaderManager::GetShader(name, configuration);
				prog.Bind();

				auto buffer = mesh->GetBuffer();
				buffer[0].first->Bind();
				bool isindexed = sn->isIndexed();
				if (isindexed) {
					mesh->GetIndexBuffer()->Bind();
				}
				int stride = sizeof(float) * 8;
				CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0));
				CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 3)));
				CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 6)));
				
				auto model = sn->relTrans;
				if (tex)
					mesh->getTexMaterial()->Bind(prog);
				else
					mesh->getColourMaterial()->Bind(prog);
				prog.setVec3("light.ambient" , light_ambient);
				prog.setVec3("light.diffuse" , light_diffuse);
				prog.setVec3("light.specular", light_specular);
				prog.setVec3("light.pos"     , light_pos);
				prog.setMat4("model", model);
				prog.setMat4("view", view);
				prog.setMat4("projection", projection);
				unsigned int count = mesh->GetCount();
				if (!isindexed) {
					CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, count));
				}
				else
				{
					CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0));
					mesh->GetIndexBuffer()->Unbind();
				}
			}
		}
		void Update() {
			ImGui::InputFloat3("Light Pos", &light_pos[0], 4);
			scr->Update(cam->GetPos(), cam->GetDir(), cam->GetView(),cam->GetProjection());
		}
		void clear() {
			dude->clear();
		}
		void init(std::shared_ptr<node::NodeBase> node) {
			ProcessNode(node);
			dude->BindLights(test);

		}
		void ProcessNode(std::shared_ptr<node::NodeBase> node) {
 			
			if (node) {
				auto vn = node->GetChilds();
				for (auto & n : vn) {
					ProcessNode(n);
				}
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
