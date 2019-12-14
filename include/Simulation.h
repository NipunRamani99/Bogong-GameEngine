#pragma once

#include "Shaders.hpp"

#include <memory>
#include "Keyboard.h"
#include "Mouse.h"

#include "Camera.h"

namespace bogong {
	class Simulation
	{
	private:
		glm::vec3 lightPos = glm::vec3(-1.0f, 0.2f, 0.0f);
		Shader m_Shader;
		std::shared_ptr<FPCamera> camera;
		public:

		Simulation();
		void Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse>& mouse, float delta);
		void Draw() const;
	};
}
