#include<glm/gtx/transform.hpp>
namespace bogong {
	class Entity {
		glm::vec3 pos;
		glm::vec3 vel;
		glm::vec3 accl;
		glm::mat4 model;
	public:
		
		Entity(glm::vec3 pos = {0,0,0}, 
			   glm::vec3 vel = { 0,0,0 }, 
			   glm::vec3 accl = { 0,0,0 },
			   glm::mat4 model = glm::mat4(1.0))
		:
			pos(pos),
			vel(vel),
			accl(accl),
			model(model)
		{
			
		}
		void Scale(glm::vec3 scale) {
			model = glm::scale(glm::mat4(1.0f), scale);
		}
		void Translate(glm::vec3 translate) {
			model = glm::translate(glm::mat4(1.0f), translate);
		}
		void Rotate(float rotate, glm::vec3 vec) {
			model = glm::rotate(glm::mat4(1.0f), rotate, vec);
		}
		void Update() {
			pos += vel;
			Translate(pos);
		}
	};
}