layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
#ifdef HAS_UV
layout(location = 2) in vec2 UV;
out vec2 texCoords;
#endif
#ifdef MATERIAL_WITH_TEX
struct Material {
sampler2D diffuse;
sampler2D specular;
float shininess;
};
#else
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
#endif
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 pos;
};
uniform Light light;
uniform Material material;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec4 FragColour;
uniform vec3 viewPos;
void main() {
	gl_Position = projection*view*model*vec4(aPos, 1.0f);
	vec3 FragPos = vec3(model*vec4(aPos, 1.0f));
	vec4 colour = vec4(1.0f, 0.0f, 1.0, 1.0f);
	vec3 norm = normalize(aNormals);
	vec3 lightDir = normalize(light.pos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);
	vec4 ambient, diffuse, specular;
#ifndef MATERIAL_WITH_TEX
	ambient = vec4(material.ambient,1.0f) * vec4(light.ambient,1.0f);
	diffuse = diff * vec4(material.diffuse,1.0f) * vec4(light.diffuse,1.0f);
	specular = vec4(spec * spec *light.specular * material.specular,1.0f);
#else
	
		ambient = texture(material.diffuse, UV).rgba*vec4(light.ambient, 1.0f);
		diffuse = diff * texture(material.diffuse, UV).rgba*vec4(light.diffuse, 1.0f);
		specular = vec4(light.specular, 1.0) * spec * texture(material.specular, UV).rgba;
	
#endif
	vec4 result = ambient + diffuse + specular;
	FragColour = result;
}