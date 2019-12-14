#include "../include/Imgui.h"
#include "../include/Engine.h"
#include "../include/Init.hpp"
#include "../include/Globals.h"
bogong::Engine::Engine()
{
	int gpuDevice = 0;
	int device_count = 0;
	Init::InitGLFW();
	window = Init::CreateWindowGL(WIDTH, HEIGHT, "Mic Check.");
	Init::SetGLFWWindow(*window, 4, 3, 3, GLFW_OPENGL_CORE_PROFILE, true);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Couldn't init glew.";
	}

	kbd->SetCallback(window);
	mouse->SetCallback(window);
	Init::InitImgui(*window);

}

void bogong::Engine::Start()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	assert((bool)!error());
	sim = std::make_shared<Simulation>();
	int display_w, display_h;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	kbd = std::make_shared<Keyboard>();
	mouse = std::make_shared<Mouse>();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

}

void bogong::Engine::Update(float deltime)
{

	sim->Update(kbd, mouse, static_cast<float>(deltime));
}

void bogong::Engine::DrawCalls() const
{
	sim->Draw();
}

void bogong::Engine::RenderEverything()
{
	DrawCalls();
	Init::Render();
	Init::EndImguiFrame();
	glfwSwapBuffers(window);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		KeepRendering = false;
	}
}

void bogong::Engine::Loop()
{
	prevTime = currentTime;
	currentTime = glfwGetTime();
	Init::StartImguiFrame();
	Init::PrepareImguiFrame();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();
	Update(currentTime - prevTime);
	float fps = 1 / (currentTime - prevTime);
	RenderEverything();

}

void bogong::Engine::End()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}
