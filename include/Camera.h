#pragma once
#include<glm/gtx/transform.hpp>
#include <memory>

namespace bogong
{
	;
	class Keyboard;
	class Mouse;
	class Camera
	{
	protected:
		glm::mat4 projection;
		glm::mat4 view;
		int width = 800;
		int height = 600;
	public:
		glm::mat4 GetProjection();

		glm::mat4 GetView();
	};

	class FPCamera : public Camera
	{
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		float cameraSpeed = 1.0f;
		int screenWidth = 800;
		int screenHeight = 600;
		float prevMouseX = 800.0f / 2.0f;
		float prevMouseY = 600.0f / 2.0f;
		float currMouseX = 0.0f;
		float currMouseY = 0.0f;
		float pitch = 0.0f;
		float yaw = -90.0f;
		bool debug = true;
	public:
		FPCamera();
		void Update(const std::shared_ptr<Keyboard> &kbd, const std::shared_ptr<Mouse> & mouse, float delta);

		glm::vec3 GetPos() { 
			return cameraPos; };
		inline void ToggleDebug() { debug = !debug; }
		glm::vec3 GetFront() { return cameraFront; }
		void ClearMouse(const std::shared_ptr<Mouse>& mouse);
	};


}
