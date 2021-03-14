layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
#ifdef HAS_UV
layout(location = 2) in vec2 UV;
out vec2 texCoords;
#endif
out vec3 Normal;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec3 FragPos;
void main() {
	gl_Position = projection * view*model*vec4(aPos, 1.0);
	Normal = mat3(transpose(inverse(model)))* aNormals;
	vec4 pos = (model * vec4(aPos, 1.0));
	FragPos = pos.xyz;
#ifdef HAS_UV
	texCoords = UV;
#endif
}