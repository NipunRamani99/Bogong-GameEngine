#pragma once
#include<map>
#include<GLFW/glfw3.h>

enum BUTTON
{
	LMB,
	RMB,
	OTHER
};
enum MOUSE_STATUS
{
	MOUSE_IS_PRESSED,
	MOUSE_IS_RELEASED,
	MOUSE_NONE
};
class Mouse
{
private:

public:
	static std::map< BUTTON, MOUSE_STATUS> MouseMap;
	Mouse()
	{
		MouseMap.insert({LMB,MOUSE_NONE});
		MouseMap.insert({RMB,MOUSE_NONE});
		MouseMap.insert({OTHER,MOUSE_NONE});


	}
	void Flush()
	{
		for (auto & Button : MouseMap)
		{
			Button.second = MOUSE_NONE;
		}
	}
	
	static void MouseCallback(GLFWwindow * p_Window, int p_Scancode, int p_Action, int p_Mod)
	{
		MOUSE_STATUS stat;
		switch (p_Action)
		{
	    	case GLFW_PRESS:
			stat = MOUSE_IS_PRESSED;
			break;
			case GLFW_RELEASE:
			stat = MOUSE_IS_RELEASED;
			break;
		    default:
				stat = MOUSE_NONE;
			break;
		}
		BUTTON butn;
		switch (p_Scancode)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
				butn = LMB;
			break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				butn = RMB;
			break;
			default:
				butn = OTHER;
			break;
		}
		MouseMap.find(butn)->second = stat;
	}
};