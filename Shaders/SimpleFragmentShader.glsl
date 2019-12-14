#version 330 core
in vec4 Colour;
out vec4 FragColour;
uniform sampler2D s;
void main() {
	FragColour = texelFetch(s,gl_Position.xy);
}