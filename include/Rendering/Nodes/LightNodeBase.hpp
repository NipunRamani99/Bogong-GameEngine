#pragma once
#include "NodeBase.hpp"

namespace bogong {
	namespace node {
		class LightNodeBase : public NodeBase{
		private:
			float intensity = 1.0f;
			glm::vec3 colour;
			bool on = true;
		public:
			LightNodeBase() = delete;
			LightNodeBase(std::string name,glm::vec3 pos);
			float GetIntensity();
			glm::vec3 GetColour();
			bool isOn();

		};
	}
}