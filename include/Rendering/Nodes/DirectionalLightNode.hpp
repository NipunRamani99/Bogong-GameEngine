#pragma once
#include "LightNodeBase.hpp"
#include "LightData.hpp"
namespace bogong {
	namespace node{
		class DirectionalLightNode : public LightNodeBase {
		public:
			DirectionalLight dl;

			DirectionalLightNode(DirectionalLight dl, glm::vec3 pos, std::string name = "DirLigh1")
				:
				LightNodeBase(name, pos),
				dl(dl)
			{
				type = Light;
				light_type = Directional;
			}

			void Bind(Program p, int i) override {
				
				p.setVec3(std::string("direction_light[" + std::to_string(i) + "].ambient").c_str(), dl.ambient);
				p.setVec3(std::string("direction_light[" + std::to_string(i) + "].diffuse").c_str(), dl.diffuse);
				p.setVec3(std::string("direction_light[" + std::to_string(i) + "].specular").c_str(), dl.specular);
				p.setVec3(std::string("direction_light[" + std::to_string(i) + "].direction").c_str(), dl.dir);
			}
			glm::vec3 GetPos() {
				return pos;
			}

		};
	}
}