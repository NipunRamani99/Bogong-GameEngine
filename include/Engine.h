#pragma once

#define WIDTH 1280
#define HEIGHT 740
#include  <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"
#include "Game.h"

namespace bogong
{


	class Engine
	{
	private:
		GLFWwindow* window;
		GLuint programID;

		std::shared_ptr<Game> game;
		std::shared_ptr<Keyboard> kbd;
		std::shared_ptr<Mouse> mouse;
		int camID = 0;
		float prevTime = 0.0;
		float currentTime = 0.0;
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
