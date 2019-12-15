#pragma once

#include "Shaders.hpp"
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"
#include "Surface.hpp"
#include "Camera.h"
#include "Globals.h"
#include "Models/Cube.hpp"
namespace bogong {
	class Game
	{
	private:
		glm::vec3 lightPos = glm::vec3(-1.0f, 0.2f, 0.0f);
		Shader m_Shader;
		std::shared_ptr<FPCamera> camera;
		std::shared_ptr<Quad> quad;
		std::shared_ptr<Cube> cube;
		public:

		Game();
		void Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse>& mouse, float delta);
		void Draw() const;
	};
}
