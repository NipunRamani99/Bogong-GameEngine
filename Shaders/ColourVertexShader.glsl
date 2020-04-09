#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
#ifdef HAS_UV
layout(location = 2) in vec2 UV;
#endif
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main() {
	gl_Position = projection*view*model*vec4(aPos, 1.0);
}