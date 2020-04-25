#pragma once
#include "LightNodeBase.hpp"
#include "LightData.hpp"
#include "../../ShaderManager.hpp"
#include <string>
namespace bogong {
	namespace node {
		class SpotLightNode : public LightNodeBase {
			SpotLight sl;
		public:
			SpotLightNode(SpotLight sl, glm::vec3 pos, std::string name = "SpotLight1")
				:
				LightNodeBase(name, pos),
				sl(sl)
			{

			}
			
			void Bind(Program p, int i) override {
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].pos").c_str(), sl.pos);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].ambient").c_str(), sl.specular);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].diffuse").c_str(), sl.diffuse);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].specular").c_str(), sl.specular);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].specular").c_str(), sl.specular);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].specular").c_str(), sl.specular);

			}
			glm::vec3 GetPos();

		};
	}
}