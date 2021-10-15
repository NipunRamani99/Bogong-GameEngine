#version 330 core
uniform samplerCube cubemap;
in vec3 fragPos;
out vec4 color;
void main() {
    color = texture(cubemap, fragPos);
}