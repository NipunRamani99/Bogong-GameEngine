
#version 330 core
uniform vec4 object_colour;
out vec4 FragColour;
#ifdef HAS_UV
out vec2 uv;
uniform sampler2D s;
#endif
void main() {
#ifndef HAS_UV
	FragColour = object_colour;
#else
	FragColour = texture(s, uv);
#endif
}