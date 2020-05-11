#pragma once
#include "LightNodeBase.hpp"
#include "LightData.hpp"
#include "../../ShaderManager.hpp"
#include <string>
namespace bogong {
	namespace node {
		class PointLightNode : public LightNodeBase{
		public:
			PointLight pl;
			PointLightNode(PointLight pl, glm::vec3 pos, std::string name="PointLight1")
				:
				LightNodeBase(name, pos),
				pl(pl)
			{
				light_type = Point;
				type = Light;
			}
			PointLightNode(PointLight pl, std::string name = "PointLight");
			void Bind(Program p,int i) override{
				p.setVec3(std::string("point_light[" + std::to_string(i) + "].pos").c_str(), pl.pos);
				p.setVec3(std::string("point_light[" + std::to_string(i) + "].ambient").c_str(), pl.ambient);
				p.setVec3(std::string("point_light[" + std::to_string(i) + "].diffuse").c_str(),  pl.diffuse);
				p.setVec3(std::string("point_light[" + std::to_string(i) + "].specular").c_str(), pl.specular);
				p.setFloat(std::string("point_light[" + std::to_string(i) + "].linear").c_str(), pl.linear);
				p.setFloat(std::string("point_light[" + std::to_string(i) + "].quadratic").c_str(), pl.quadratic);
				p.setFloat(std::string("point_light[" + std::to_string(i) + "].constant").c_str(), pl.constant);

			}

		};
	}
}