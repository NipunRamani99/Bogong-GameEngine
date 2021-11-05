#include "../include/Camera.h"
#include "../include/Keyboard.h"
#include "../include/Mouse.h"
#include "../Imgui/imgui.h"
#include<string>
namespace bogong {
	class Mouse;
}
glm::mat4 bogong::Camera::GetProjection()
{
	return projection;
}
glm::mat4 bogong::Camera::GetView()
{
	return view;
}
bogong::FPCamera::FPCamera()
{
	projection = glm::perspective(glm::radians(45.0f), float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
	view = lookAt(cameraPos, cameraPos + cameraFront, up);
}
void bogong::FPCamera::ClearMouse(const std::shared_ptr<Mouse>& mouse)
{
	prevMouseX = mouse->x;
	prevMouseY = mouse->y;
	currMouseX = mouse->x;
	currMouseY = mouse->y;
}
void bogong::FPCamera::Update(const std::shared_ptr<Keyboard>& kbd, const std::shared_ptr<Mouse>& mouse, float delta)
{
	float camSpeed =0.0f;
	if (kbd->isKeyPressed(KEY::KEY_SHIFT)) {
		camSpeed = 2.0f*delta;
	}
	else {
		camSpeed = cameraSpeed * delta;
	}

	if (kbd->isKeyPressed(KEY_W) || kbd->isKeyRepeating(KEY_W))
	{
		cameraPos += camSpeed * cameraFront;
	}
	if (kbd->isKeyPressed(KEY_S) || kbd->isKeyRepeating(KEY_S))
	{
		cameraPos -= camSpeed * cameraFront;
	}
	if (kbd->isKeyPressed(KEY_A) || kbd->isKeyRepeating(KEY_A))
	{
		cameraPos -= normalize(cross(cameraFront, up)) * camSpeed;
	}
	if (kbd->isKeyPressed(KEY_D) || kbd->isKeyRepeating(KEY_D))
	{
		cameraPos += normalize(cross(cameraFront, up)) * camSpeed;
	}
	if (kbd->isKeyPressed(KEY_SPACE) || kbd->isKeyRepeating(KEY_SPACE))
	{
		cameraPos += normalize(up) * camSpeed;
	}
	if (kbd->isKeyPressed(KEY_CTRL)|| kbd->isKeyRepeating(KEY_CTRL))
	{
		cameraPos += normalize(-up) * camSpeed;
	}
	prevMouseX = currMouseX;
	prevMouseY = currMouseY;
	currMouseX = mouse->x;
	currMouseY = mouse->y;
	float xoffset = currMouseX - prevMouseX;
	float yoffset = prevMouseY - currMouseY;

	xoffset = 0.05*xoffset;
	yoffset = 0.05*yoffset;
	yaw += xoffset;
	pitch += yoffset;
	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	glm::vec3 direction;
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	view = lookAt(cameraPos, cameraPos + cameraFront, up);
}
