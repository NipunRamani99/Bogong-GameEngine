#pragma once
#include<GLFW/glfw3.h>
#include<array>
#include<map>
#include<functional>
/*
*Keyboard Class.
*
*/
enum KEY
{
	KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,
	KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,
	KEY_M,KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,
	KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,
	KEY_Y,KEY_Z,KEY_UP,KEY_DOWN,KEY_LEFT,
	KEY_RIGHT,KEY_SHIFT,KEY_CTRL,KEY_ESC
};
enum STATUS
{
	IS_PRESSED,
	IS_RELEASED,
	IS_REPEATING,
	NONE
	
};
class Keyboard
{   //Container to hold if any key was pressed or not.
private:
	static std::map<int, STATUS> KeyMap;

public:
	Keyboard()
	{		
		//Initialize the Container and set every isPressed value to false.
		for (int i = 0; i < 36; i++)
		{
			KeyMap.insert({i,NONE});
		}
	}
	void Flush()
	{
		for (auto & Key:Keyboard::KeyMap)
		{
			Key.second = NONE;
		}
	}
	void SetCallback(GLFWwindow * p_Window)
	{
		
		glfwSetKeyCallback(p_Window,Keyboard::Callback);
	}
	static void Callback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int mods)
	{
		if (p_Key >= 65 && p_Key <= 90)
		{
			p_Key = p_Key - 65;
			STATUS stat;
			if (p_Action == GLFW_PRESS)
				stat = IS_PRESSED;
			if (p_Action == GLFW_RELEASE)
				stat = IS_RELEASED;
			if (p_Action == GLFW_REPEAT)
				stat = IS_REPEATING;
		    
			Keyboard::KeyMap.find(p_Key)->second = stat;
		}
	}

	bool KeyIsPressed(KEY p_Key)
	{
		bool RetnVal = false;
		if (Keyboard::KeyMap.at(p_Key) == IS_PRESSED)
			RetnVal = true;
	    return RetnVal;
	}
	bool KeyIsRepeating(KEY p_Key)
	{
		bool RetnVal = false;
		if (Keyboard::KeyMap.at(p_Key) == IS_REPEATING)
			RetnVal = true;
		return RetnVal;
	}
	bool KeyIsReleased(KEY p_Key)
	{
		bool RetnVal = false;
		if (Keyboard::KeyMap.at(p_Key) == IS_RELEASED)
			RetnVal = true;
		return RetnVal;
	}
};
