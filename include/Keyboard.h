#pragma once
#include<GLFW/glfw3.h>
#include<array>
#include<map>
#include<functional>
namespace bogong {

	enum KEY
	{
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F,
		KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L,
		KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
		KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X,
		KEY_Y, KEY_Z, KEY_UP, KEY_DOWN, KEY_LEFT,
		KEY_RIGHT, KEY_SHIFT, KEY_CTRL, KEY_ESC, KEY_SPACE
	};
	enum STATUS
	{
		IS_PRESSED,
		IS_RELEASED,
		IS_REPEATING,
		NONE

	};
	class Keyboard
	{
	private:
		static std::map<int, STATUS> KeyMap;

	public:
		Keyboard();

		void SetCallback(GLFWwindow* p_Window);

		static void Callback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int mods);
		void debugOutput();
		bool isKeyPressed(KEY p_Key);

		bool isKeyRepeating(KEY p_Key);

		bool isKeyReleased(KEY p_Key);
	};
}