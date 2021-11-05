
#include "../Imgui/imgui.h"
#include <string>
#include "../include/Keyboard.h"

namespace bogong {
	std::map<int, STATUS> Keyboard::KeyMap = std::map<int, STATUS>();

	Keyboard::Keyboard()
	{
		//Initialize the Container and set every isPressed value to false.
		for (int i = 0; i < 36; i++)
		{
			KeyMap.insert({ i, NONE });
		}
	}
	void Keyboard::debugOutput()
	{
		int i = 0;
		for (auto & Key : Keyboard::KeyMap)
		{
			char charac = i + 'A';
			i++;
			std::string str1 = std::string(1, charac);
			std::string str = str1 + " : " + std::to_string(Key.second);
			ImGui::Text(str.c_str());
		}
	}

	void Keyboard::SetCallback(GLFWwindow* p_Window)
	{
		glfwSetKeyCallback(p_Window, Keyboard::Callback);
	}

	void Keyboard::Callback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int mods)
	{
		STATUS stat;
		if (p_Action == GLFW_PRESS)
			stat = IS_PRESSED;
		if (p_Action == GLFW_RELEASE)
			stat = IS_RELEASED;
		if (p_Action == GLFW_REPEAT)
			stat = IS_REPEATING;

		if (p_Key >= 65 && p_Key <= 90)
		{
			p_Key = p_Key - 65;	
			Keyboard::KeyMap.find(p_Key)->second = stat;
		}
		if (p_Key == GLFW_KEY_LEFT_SHIFT) {
			p_Key = KEY::KEY_SHIFT;
			Keyboard::KeyMap.find(p_Key)->second = stat;
		}
		if (p_Key == GLFW_KEY_LEFT_CONTROL) {
			p_Key = KEY::KEY_CTRL;
			Keyboard::KeyMap.find(p_Key)->second = stat;
		}
		if (p_Key == GLFW_KEY_SPACE) {
			p_Key = KEY::KEY_SPACE;
			Keyboard::KeyMap.find(p_Key)->second = stat;
		}
	}

	bool Keyboard::isKeyPressed(KEY p_Key)
	{
		bool RetnVal = false;
		if (Keyboard::KeyMap.at(p_Key) == IS_PRESSED)
			RetnVal = true;
		return RetnVal;
	}

	bool Keyboard::isKeyRepeating(KEY p_Key)
	{
		bool RetnVal = false;
		if (Keyboard::KeyMap.at(p_Key) == IS_REPEATING)
			RetnVal = true;
		return RetnVal;
	}

	bool Keyboard::isKeyReleased(KEY p_Key)
	{
		bool RetnVal = false;
		if (Keyboard::KeyMap.at(p_Key) == IS_RELEASED)
			RetnVal = true;
		return RetnVal;
	}
}
