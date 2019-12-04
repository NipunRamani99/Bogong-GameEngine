#pragma once
#include "../include/Mouse.h"
namespace bogong {
	std::map<BUTTON, MOUSE_STATUS> Mouse::MouseMap = std::map<BUTTON, MOUSE_STATUS>();
	double Mouse::x = 0;
	double Mouse::y = 0;
}
