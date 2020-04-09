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
#include <GLFW/glfw3.h>
#include "Rendering/SampleScene.hpp"
namespace bogong {
	class Game
	{
	private:
		

		std::shared_ptr<SampleScene> scene;
		bool isMouseEnabled = false;
		public:
		Game();
		void Update(const std::shared_ptr<bogong::Keyboard> &kbd, const std::shared_ptr<bogong::Mouse>& mouse, float delta,GLFWwindow * window);
		void Draw() const;
	};
}
