#pragma once
#include <iostream>
#include "Nodes/ShapeNode.hpp"
#include "Nodes/LightNodeBase.hpp"
#include "Mesh.hpp"
#include <queue>
namespace bogong {

	struct RenderQueueItem {
		Mesh *node;
		glm::mat4 trans = glm::mat4(1.0f);
		int priority = 1;
	};
	struct Light {

	};
	class RendererDude {
		
	private:
		//has a queue of renderable dudes with what to render using
		std::vector<RenderQueueItem> render_queue;
		std::vector<node::LightNodeBase*> lights;
		std::shared_ptr<VertexArray> vao;
		//keep count of # of lights per type
		int num_spot_lights  = 0;
		int num_point_lights = 0;
		int num_dir_lights   = 0;
		int idx_spot  = 0;
		int idx_dir   = 0;
		int idx_point = 0;
		int curr_priority = 0;
		bool  first = true;
	public:
		RendererDude() {
			vao = std::make_shared<VertexArray>();
			vao->Bind();
			vao->Unbind();
		}
		void AddShapeNode(RenderQueueItem & item){
			render_queue.push_back(item);

		}
		void AddLight(node::LightNodeBase * light){
			if (light->light_type == node::Point) {
				num_point_lights++;
			}
			if (light->light_type == node::Spot) {
				num_spot_lights++;
			}
			if (light->light_type == node::Directional) {
				num_dir_lights++;
			}
			lights.push_back(light);
		}
		
		void BindLights(Program p){
			p.Bind();
			p.setInt("num_point_light", num_point_lights);
			p.setInt("num_spot_light", num_spot_lights);
			p.setInt("num_direction_light", num_dir_lights);
			idx_dir   = 0;
			idx_point = 0;
			idx_spot  = 0;
			for (auto  light : lights) {
				switch (light->light_type) {
				case node::Point: {
					light->Bind(p, idx_point);
					idx_point++;
					break;
				}
				case node::Directional: {
					light->Bind(p, idx_dir);
					idx_dir++;
					break;
				}
				case node::Spot: {
					light->Bind(p, idx_spot);
					idx_spot++;
					break;
				}
				default:
					break;
				}
			}
		}

		void clear() {
			lights.resize(0);
			render_queue.resize(0);
			num_spot_lights = 0;
			num_dir_lights = 0;
			num_point_lights = 0;
		}
		void prerender() {
			std::sort(render_queue.begin(), render_queue.end(), [](RenderQueueItem &a, RenderQueueItem&b) {
				return a.priority > b.priority;
			});
		}
void Render(Program p,glm::vec3 cam_pos,glm::mat4 projection, glm::mat4 view) {
	
	p.Bind();
	vao->Bind();
	p.setVec3("cam_pos", cam_pos);
	
	for (auto& it : render_queue) {
		auto mesh = it.node;
		auto buffer = mesh->GetBuffer();
		buffer[0].first->Bind();
		bool isindexed = true;
		if (isindexed) {
			mesh->GetIndexBuffer()->Bind();
		}
		auto model = it.trans;
		if (true)
		{
			mesh->getTexMaterial()->Bind(p);	
		}
		else
			mesh->getColourMaterial()->Bind(p);
		int stride = sizeof(float) * 8;
		CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0));
		CHECK_GL_ERROR(glEnableVertexAttribArray(0));
		CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 3)));
		CHECK_GL_ERROR(glEnableVertexAttribArray(1));
		CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (const void *)(sizeof(float) * 6)));
		CHECK_GL_ERROR(glEnableVertexAttribArray(2));

		p.setMat4("model", model);
		p.setMat4("view",  view);
		p.setMat4("projection", projection);
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
		void DrawItems() {

		}
		
	};
}