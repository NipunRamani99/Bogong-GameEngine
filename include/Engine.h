#pragma once

#define WIDTH 800
#define HEIGHT 600
#include  <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"
#include "Simulation.h"

namespace bogong
{


	class Engine
	{
	private:
		GLFWwindow* window;
		GLuint programID;

		std::shared_ptr<Simulation> sim;
		std::shared_ptr<Keyboard> kbd;
		std::shared_ptr<Mouse> mouse;
		int camID = 0;
		double prevTime = 0.0;
		double currentTime = 0.0;
	public:

		bool KeepRendering = true;
		Engine();
		void Start();
		void Update(float deltime);
		void DrawCalls() const;
		void RenderEverything();
		void Loop();
		void End();
	};
}
