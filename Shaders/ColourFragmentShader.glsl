uniform vec4 object_colour;
out vec4 FragColour;
#ifdef HAS_UV
in vec2 texCoords;
uniform sampler2D s;
#endif
void main() {
#ifndef HAS_UV
	FragColour = object_colour;
#else
	FragColour = texture(s, texCoords);
#endif
}