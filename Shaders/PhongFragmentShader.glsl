#version 330 core
in vec3 Normals;
out vec4 FragColour;
uniform vec4 object_colour;
uniform vec3 light_pos;
uniform float ambient_str = 1.0f;
void main() {
	FragColour = vec4((object_colour.xyz)*ambient_str,1.0f);

}