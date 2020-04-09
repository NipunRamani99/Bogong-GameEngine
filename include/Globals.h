#pragma once
#define ENABLE_GL_CHECK 0

#include <iostream>
#include <string>
#include <sstream>
#define CHECK_GL_ERROR(glFunc) \
{ \
glFunc; \
int e = glGetError(); \
if (e != 0) \
{ \
const char * errorString = ""; \
switch (e) \
{ \
case GL_INVALID_ENUM:       errorString = "GL_INVALID_ENUM";        break; \
case GL_INVALID_VALUE:      errorString = "GL_INVALID_VALUE";       break; \
case GL_INVALID_OPERATION:  errorString = "GL_INVALID_OPERATION";   break; \
case GL_INVALID_FRAMEBUFFER_OPERATION:  errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";   break; \
case GL_OUT_OF_MEMORY:      errorString = "GL_OUT_OF_MEMORY";       break; \
default:                                                            break; \
} \
std::stringstream ss; \
std::string func_call(#glFunc); \
ss << "OpenGL error "<<errorString<<" while calling "<<func_call<<" in function "<< __FUNCTION__<<" at line "<< __LINE__ <<"\n";\
std::cout << ss.str(); \
} \
}\

namespace bogong {
	bool error();
}