#pragma once
#include "Init.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.h"
#include "Imgui.h"
#include "Callbacks.h"
#include "Simulation.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"

class Engine
{
private:
	GLFWwindow * window;
	GLuint programID;
	IsoCamera * cam;
	FreeCamera * free;
	Simulation sim;
	Keyboard kbd;
	int camID = 0;
public:

	bool KeepRendering = true;
	Engine()
	{
		int gpuDevice = 0;
		int device_count = 0;
		Init::InitGLFW();
		window = Init::CreateWindow(800, 600, "Mic Check.");
		Init::SetGLFWWindow(*window, 4, 3, 3, GLFW_OPENGL_CORE_PROFILE, true);
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			std::cout << "Couldn't init glew.";
		}
		cam = new IsoCamera(window, 800, 600);
		free = new FreeCamera(800, 600);

		Callbacks::camID = camID;
		Callbacks::freecam = free;
		Callbacks::cam = cam;
		kbd.SetCallback(window);
		glfwSetCursorPosCallback(window, Callbacks::mousePositionCallback);
		glfwSetMouseButtonCallback(window, Callbacks::mouseButtonCallback);
		Init::InitImgui(*window);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

	}
	void Start()
	{
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		Shader shader;
		shader.LoadShader("vertexShader.glsl", ShaderType::VERTEX);
		shader.LoadShader("fragmentShader.glsl", ShaderType::FRAGMENT);
		shader.LoadProgram();
		programID = shader.GetProgramID();
		glUseProgram(programID);
		ICallbacks::AddShader(shader);
		assert(( bool )!error());
		sim = std::move(Simulation(shader));
		error();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glClearColor(Init::clear_color.x, Init::clear_color.y, Init::clear_color.z, Init::clear_color.w);
		glViewport(0, 0, display_w, display_h);
		cam->setShader(shader);
		free->SetShader(shader);
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);
		shader.setBool("isTextured", false);
	}
	void Update()
	{
		sim.Update();
		glm::vec3 pos = free->GetCameraPos();
		ImGui::Text("X: %.1f Y: %.1f Z: %.1f", pos.x, pos.y, pos.z);
		if(ImGui::InputFloat("View Radius: ", &cam->radius, 0.5, 0.5, 4))
		{
			cam->updateCamera();
		}
		if (ImGui::ColorEdit3("clear color", (float*)&Init::clear_color))
		{
			glClearColor(Init::clear_color.x, Init::clear_color.y, Init::clear_color.z, Init::clear_color.w);
		}
		if (camID == 0) {
			if (ImGui::Button("Switch Camera"))
			{
				if (camID == 0)
				{
					Callbacks::camID = 1;
					camID = 1;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


				}
			}
		}
		else {
			ImGui::Text("Press K to switch back.");
			if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			{

				Callbacks::camID = 0;
				camID = 0;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			}
		}
	}
	void DrawCalls()
	{
		sim.Draw();
	}
	void RenderEverything()
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
	void Loop()
	{
		kbd.Flush();
	
		glfwPollEvents();
		if (kbd.KeyIsPressed(KEY_A))
		{
			std::cout << "KEY A Is Pressed.";
		}
		if (kbd.KeyIsRepeating(KEY_A))
		{
			std::cout<< "KEY A Is Repeating.";
		}
		if (kbd.KeyIsReleased(KEY_A))
		{
			std::cout << "KEY A Is Released.";
		}
		Init::StartImguiFrame();
		Init::PrepareImguiFrame();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		
		Update();
		RenderEverything();
	}
	void End()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(window);
		glfwTerminate();

	}
};
