#version 330 core
uniform vec4 object_colour;
out vec4 FragColour;
void main() {
	FragColour = object_colour;
}