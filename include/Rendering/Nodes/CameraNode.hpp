#pragma once
#include "NodeBase.hpp"
namespace bogong {
	namespace node {
		class CameraNode : public NodeBase {
		private:
			glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
			float nearF = 0.1f;
			float farF = 300.f;
			glm::vec3 target = glm::vec3(0.f, 0.f, 1.f);
			glm::mat4 view;
			glm::mat4 projection;

		public:
			CameraNode() = delete;
			CameraNode(const CameraNode & camnode);
			~CameraNode() {}
			void SetPos(glm::vec3 pos);
			void setNearF(float nearF);
			void setFarF(float farF);
			void setTarget(glm::vec3 ta);
		};
	}
}