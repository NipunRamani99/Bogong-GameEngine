layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
#ifdef HAS_UV
layout(location = 2)in vec2 aUV;
out vec2 UV;
#endif
uniform vec3 light_pos;
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 FragPos;
void main() {
	gl_Position = projection*view*model*vec4(aPos, 1.0);
	FragPos = vec3(model*vec4(aPos, 1.0));
#ifdef HAS_UV
	UV = aUV;
#endif
	Normal = aNormal;
}