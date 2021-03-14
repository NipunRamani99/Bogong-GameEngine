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
#include "../ImGuiPane.hpp"
#include <vector>
#include <string>
namespace bogong {
	
	class SceneManager {

	private:
		VertexArray vao2;
		VertexArray skybox_vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<VertexBuffer> skybox_vbo;
		std::shared_ptr<FPCamera> cam;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 light_pos      = glm::vec3(4.0f, 5.0f, 6.0f);
		glm::vec3 light_ambient  = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 light_diffuse  = glm::vec3(0.8f, 0.8f, 0.8f);
		glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		Program output;
		Program test;
		Program skybox_shader;
		Program environment_shader;
		float skyboxVertices[108] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		float speed = 0.0f;
		float cube_vertices[288] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,

		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,


		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f

		};
		std::shared_ptr<Framebuffer> frame;
		float quadVertices[24] = {  -1.0f,  1.0f,  0.0f, 1.0f,
									-1.0f, -1.0f,  0.0f, 0.0f,
									 1.0f, -1.0f,  1.0f, 0.0f,
									-1.0f,  1.0f,  0.0f, 1.0f,
									 1.0f, -1.0f,  1.0f, 0.0f,
									 1.0f,  1.0f,  1.0f, 1.0f};
		bool attribEnabled = false;
		std::shared_ptr<RendererDude> dude;
		//std::shared_ptr<TexturedCube> tex_cube;
		std::vector<node::NodeBase*> vecNodes;
		int node_idx = 0;
		int selected_node = -1;
		bool open = false;
		unsigned int skybox_tex = 0;
		std::shared_ptr<Texture> cube_texture;
		VertexArray cube_vao;
		Program cube_shader;
		std::shared_ptr<VertexBuffer> cube_vbo;
		std::vector<std::string> skybox_faces = {
				"right.jpg",
				"left.jpg",
				"top.jpg",
				"bottom.jpg",
				"front.jpg",
				"back.jpg"
		};
		std::shared_ptr<ImGuiPane> pane;
		std::string show_fps_msg = "Show FPS";
		bool show_fps = false;
		bool show_scene_tree = false;
		unsigned int loadCubemap()
		{
			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			int width, height, nrChannels;
			for (unsigned int i = 0; i < skybox_faces.size(); i++)
			{
				std::string path = "./assets/skybox/lake/" + skybox_faces[i];
				unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, height, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					));
					stbi_image_free(data);
				}
				else
				{
					std::cout << "Cubemap tex failed to load at path: " << skybox_faces[i] << std::endl;
					stbi_image_free(data);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			return textureID;
		}
	public:

		float time = 0.0f;
		SceneManager()
		{
			vbo = std::make_shared<VertexBuffer>(quadVertices, sizeof(float) * 24);
			//frame = std::make_shared<Framebuffer>(1280,640);
			//frame->InitDepthStencilAndColour();
			Configuration config;
			output = ShaderManager::GetShader("Frame", config);
			config.macros.push_back("HAS_UV");
			config.macros.push_back("MATERIAL_WITH_TEX");
			test = ShaderManager::GetShader("MultipleLight", config);
			dude = std::make_shared<RendererDude>();
			//Create texture for skybox
			skybox_tex = loadCubemap();
			//Create a cube for skybox
			// Create a VAO
			skybox_vao.Bind();
			// Create a vbo
			//	Buffer the vertices
 			skybox_vbo = std::make_shared<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices) * sizeof(float));
			// Create a vertex buffer layout
			skybox_vbo->Bind();
			int stride = sizeof(float) * 3;
			CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void *)0));
			CHECK_GL_ERROR(glEnableVertexAttribArray(0));
			// Create the shader
			Configuration skybox_config;
			skybox_shader = ShaderManager::GetShader("Skybox", skybox_config);
			Configuration cube_config;
			//Create a textured cube
			//Load the texture
			cube_texture =	TextureManager::make_texture("./assets/models/crate/container2.png");
			//Create a vao
			cube_vao.Bind();
			//Create a vbo
			cube_vbo = std::make_shared<VertexBuffer>(cube_vertices, sizeof(cube_vertices) * sizeof(float));
			cube_vbo->Bind();
			stride = sizeof(float) * 8;
			CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void *)0));
			CHECK_GL_ERROR(glEnableVertexAttribArray(0));
			CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 3)));
			CHECK_GL_ERROR(glEnableVertexAttribArray(1));
			CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 6)));
			CHECK_GL_ERROR(glEnableVertexAttribArray(2));
			//Create
			cube_shader = ShaderManager::GetShader("Colour", config);
			environment_shader = ShaderManager::GetShader("Environmental", config);
			pane = std::make_shared<ImGuiPane>("FPS", glm::vec2(0, 0), glm::vec2(300, 300));
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
			
			if (selected_node != -1) {
				if (show_scene_tree) {
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
			}
			vecNodes[0]->UpdateTree();
			ImGui::InputFloat("Input speed bro: ", &speed, 0.001);
			if(ImGui::Button(show_fps_msg.c_str())) {
				show_fps = !show_fps;
			}
			if (ImGui::Button("Toggle Scene Tree View")) {
				show_scene_tree = !show_scene_tree;
			}
			pane->UpdatePane();
			if (show_fps)
			{
				pane->Render();
			}
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
			if (show_scene_tree) {
				ImGui::Begin("Scene");
				open = ImGui::TreeNode("Sample Scene");
				ProcessNode(node, open);
				if (open)  ImGui::TreePop();
				ImGui::End();
				dude->BindLights(test);
				dude->BindLights(test);
			}
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
		//	dude->prerender();
			//Draw Skybox
			// Bind VAO
			//glDisable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
			skybox_vao.Bind();
			// Bind the shader
			skybox_shader.Bind();
			glm::mat4 view = glm::mat4(glm::mat3(cam->GetView()));
			glm::mat4 model = glm::mat4(1.0);
			model = glm::rotate(model, time, glm::vec3(0., 1., 0.));
			skybox_shader.setMat4("model", model);
			skybox_shader.setMat4("view", view);
			skybox_shader.setMat4("projection", cam->GetProjection());
			skybox_shader.setFloat("u_time", time);
			CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex));
			// Draw
			CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			glDepthMask(GL_TRUE);
			//dude->Render(test,cam->GetPos() ,cam->GetProjection(), cam->GetView());
			//Render the box
			//Bind VAO
			cube_vao.Bind();
			//Bind Shader
			environment_shader.Bind();
			//Set Shader variables
			
			model = glm::rotate(model, time, glm::vec3(0.,1.,0.));
			environment_shader.setMat4("model", model);
			environment_shader.setMat4("view", cam->GetView());
			environment_shader.setMat4("projection", cam->GetProjection());
			environment_shader.setVec3("CamPos", cam->GetPos());
			//Bind Texture
			CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex));
			cube_texture->Bind();
			//Draw
			CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));

			time += speed;
		}
	};

}
