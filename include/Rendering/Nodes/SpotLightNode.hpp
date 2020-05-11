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
				type = Spot;
			}
			
			void Bind(Program p, int i) override {
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].position").c_str(), sl.pos);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].ambient").c_str(), sl.specular);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].diffuse").c_str(), sl.diffuse);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].specular").c_str(), sl.specular);
				p.setVec3(std::string("spot_light[" + std::to_string(i) + "].direction").c_str(), sl.dir);
				p.setFloat(std::string("spot_light[" + std::to_string(i) + "].cutOff").c_str(), sl.innercutoff);
				p.setFloat(std::string("spot_light[" + std::to_string(i) + "].outerCutOff").c_str(), sl.outercutoff);
				p.setFloat(std::string("spot_light[" + std::to_string(i) + "].linear").c_str(), sl.linear);
				p.setFloat(std::string("spot_light[" + std::to_string(i) + "].quadratic").c_str(), sl.quadratic);
				p.setFloat(std::string("spot_light[" + std::to_string(i) + "].constant").c_str(), sl.constant);


			}
			glm::vec3 GetPos() {
				return pos;
			}

		};
	}
}