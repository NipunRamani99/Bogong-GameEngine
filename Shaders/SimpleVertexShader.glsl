#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColour;
out vec4 Colour;
void main() {
	gl_Position = vec4(aPos, 0.0);
	Colour = aColour;
}