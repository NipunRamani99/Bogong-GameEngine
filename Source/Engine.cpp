#include "../include/Engine.h"
#include "../include/Init.hpp"
#include "../include/Globals.h"
#include "../include/Imgui.h"
namespace bogong {
	namespace Init {

		void InitImgui(GLFWwindow & window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();
			const char* glsl_version = "#version 330";
			// Setup Platform/Rendeer bindings
			ImGui_ImplGlfw_InitForOpenGL(&window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		void StartImguiFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void Render()
		{
			ImGui::Render();
		}
		void EndImguiFrame()
		{

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		}
	}
}
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
	pane = std::make_shared<ImGuiPane>("Test Pane", glm::vec2(0, 0), glm::vec2(300, 300));
}

void bogong::Engine::Start()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	assert((bool)!error());
	game = std::make_shared<Game>();
	assert(!error());
	int display_w, display_h;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	
	glViewport(0, 0, display_w, display_h);
	kbd = std::make_shared<Keyboard>();
	mouse = std::make_shared<Mouse>();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);  
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
}

void bogong::Engine::Update(float deltime)
{

	game->Update(kbd, mouse, static_cast<float>(deltime),window);
	assert(!error());
	pane->UpdatePane();
}

void bogong::Engine::DrawCalls(float deltatime) const
{
	game->Draw(deltatime);
}

void bogong::Engine::RenderEverything(float deltatime)
{

	DrawCalls(deltatime);
	pane->Render();
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
	currentTime = (float)glfwGetTime();
	Init::StartImguiFrame();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();
	Update(currentTime - prevTime);
	//Timer::LogTimeElapsed("Update");
	RenderEverything(currentTime-prevTime);
	
}

void bogong::Engine::End()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}
