#pragma once
#include "Init.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.h"
#include "Imgui.h"
#include "Callbacks.h"
#include "Camera.h"
#include "GuideLines.h"
#include "Simulation.h"

class App
{
private:
	GLFWwindow * window;
	GLuint programID;
	IsoCamera * cam;
	FreeCamera * free;
	Simulation sim;
	int camID = 0;
public:

	bool KeepRendering = true;
	App()
	{
		int err = 0;
		cudaDeviceProp deviceProp;
		int gpuDevice = 0;
		int device_count = 0;
		cudaGetDeviceCount(&device_count);
		Init::InitGLFW();
		window = Init::CreateWindow(800, 600, "Mic Check.");
		Init::SetGLFWWindow(*window, 4, 3, 3, GLFW_OPENGL_CORE_PROFILE, true);
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			std::cout << "Couldn't init glew.";
		}
		if (err != 0)
		{
		}
		cudaGLSetGLDevice(0);

		cam = new IsoCamera(window, 800, 600);
		free = new FreeCamera(800, 600);

		Callbacks::camID = camID;
		Callbacks::freecam = free;
		Callbacks::cam = cam;

		glfwSetKeyCallback(window, Callbacks::keyCallback);
		glfwSetCursorPosCallback(window, Callbacks::mousePositionCallback);
		glfwSetMouseButtonCallback(window, Callbacks::mouseButtonCallback);


		Init::InitImgui(*window);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
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
		error();
		glUseProgram(programID);
		sim = std::move(Simulation(shader,*free, *cam));
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
	void makeTriangle()
	{
	}
	void Update()
	{
		sim.Update();
		if (ImGui::InputFloat("View Radius: ", &cam->radius, 0.5, 0.5, 4))
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
		glfwPollEvents();
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
