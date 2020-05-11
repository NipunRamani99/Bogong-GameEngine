layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
#ifdef HAS_UV
layout(location = 2) in vec2 aTexCoords;
#endif
uniform vec3 light_pos;
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 FragCoord;
#ifdef HAS_UV
out vec2 UV;
#endif
void main() {
	gl_Position = (projection*view*model*vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragCoord = vec3(model * vec4(aPos, 1.0));
#ifdef HAS_UV
	UV = aTexCoords;
#endif
}