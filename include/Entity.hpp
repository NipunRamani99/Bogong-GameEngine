#pragma once
#include<glm/gtx/transform.hpp>
namespace bogong {
	class Entity {
	protected:
		
	public:
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 vel;
		glm::vec3 accl;
		glm::mat4 model;
		glm::vec3 rotate;
		Entity(glm::vec3 pos    = { 0,0,0 },
			   glm::vec3 scale  = { 1,1,1 },
			   glm::vec3 vel    = { 0,0,0 },
			   glm::vec3 accl   = { 0,0,0 },
			   glm::mat4 model  = glm::mat4(1.0),
			   glm::vec3 rotate = glm::vec3(0.0)
		)
		:
			pos(pos),
			scale(scale),
			vel(vel),
			accl(accl),
			model(model),
			rotate(rotate)
		{
			UpdateModel();
		}
		void Scale(glm::vec3 scale_) {
			scale = scale_;
		}
		void Translate(glm::vec3 translate) {
			pos = translate;
		}

		/* rotate is in degrees */
		void Rotate(glm::vec3 rotate_) {
			rotate = rotate_;
		}
		
		void UpdateModel() {
			model = glm::mat4(1.0f);
			model = glm::scale(model, scale);
			model = glm::translate(model, pos);
			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	};
}