#version 330 core
in vec3 Normal;
out vec4 FragColour;
uniform vec4 object_colour;
uniform vec3 light_pos;
uniform vec3 light_colour;
uniform float ambient_str = 1.0f;
uniform float specular_str = 1.0f;
vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
in vec3 FragPos;
uniform vec3 viewPos;
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;
uniform Material material;
void main() {
	vec4 colour = vec4(1.0f,0.0f,1.0,1.0f);
	vec3 ambient = material.ambient * light.ambient;

	vec3 norm      = normalize(Normal);
	vec3 lightDir  = normalize(light_pos - FragPos);
	float diff     = max(dot(norm, lightDir), 0.0);
	vec3 diffuse   = diff * material.diffuse * light.diffuse;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);
	vec3 specular = specular_str*spec*light.specular*material.specular;
	vec3 result = (ambient + diffuse+specular);
	FragColour  = vec4(result.xyz, 1.0);
}