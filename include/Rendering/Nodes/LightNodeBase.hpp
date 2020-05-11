#pragma once
#include "NodeBase.hpp"
#include "../../ShaderManager.hpp"
namespace bogong {
	namespace node {
		enum LightType {
			Point,
			Directional,
			Spot
		};
		class LightNodeBase : public NodeBase{
		private:
			
		public:
			LightType light_type;
			LightNodeBase() = delete;
			LightNodeBase(std::string name,glm::vec3 pos);
			virtual void Bind(Program p, int idx) {
			
			};

		};
	}
}