#pragma once

#include "Shaders.hpp"
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"
#include "Surface.hpp"
#include "Camera.h"
#include "Globals.h"
#include "Models/Cube.hpp"
#include "Models/Plane.hpp"
namespace bogong {
	class Game
	{
	private:
		glm::vec3 light_pos = glm::vec3(-1.0f, 0.2f, 0.0f);
		glm::vec4 object_colour = glm::vec4(0.5f, 0.1f, 0.4f, 1.0f);
		Shader m_Shader;
		Shader phong_shader;
		std::shared_ptr<FPCamera> camera;
		std::shared_ptr<Plane> plane;
		std::shared_ptr<Cube> cube;
		public:

		Game();
		void Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse>& mouse, float delta);
		void Draw() const;
	};
}
