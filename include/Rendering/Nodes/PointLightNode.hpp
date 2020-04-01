#pragma once
#include "LightNodeBase.hpp"

namespace bogong {
	namespace node {
		class PointLightNode : public LightNodeBase {
			glm::vec3 pos = glm::vec3(1.0f, 1.0f, 1.0f);
		public:
			PointLightNode() = delete;
			PointLightNode(std::string name);
			glm::vec3 GetPos();

		};
	}
}