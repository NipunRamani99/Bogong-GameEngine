#pragma once
#include "LightNodeBase.hpp"
#include "LightData.hpp"
namespace bogong {
	namespace node {
		class PointLightNode : public NodeBase {
			PointLight pl;
		public:
			PointLightNode() = delete;
			PointLightNode(PointLight pl, std::string name = "PointLight");
			PointLight & getData();
			glm::vec3 GetPos();
		};
	}
}