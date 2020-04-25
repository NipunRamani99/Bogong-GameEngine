#pragma once
#include "NodeBase.hpp"
#include "../../ShaderManager.hpp"
namespace bogong {
	namespace node {
		class LightNodeBase : public NodeBase{
		private:
		public:
			LightNodeBase() = delete;
			LightNodeBase(std::string name,glm::vec3 pos);
			virtual void Bind(Program p, int idx) {
			
			};
		};
	}
}