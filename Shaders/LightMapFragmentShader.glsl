#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;
out vec4 FragColour;
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform vec3 light_pos;
uniform Light light;
uniform sampler2D s;
uniform sampler2D map;
void main() {
	vec3 colour = vec3(1.0f);
	vec3 ambient = light.ambient*texture(s,TexCoords).rgb;
	vec3 norm = normalize(Normal);
	vec3 lr = normalize(light_pos - FragPos);
	float dif = max(dot(lr, norm),0.0);
	vec3 diff = dif * light.diffuse*texture(s, TexCoords).rgb;
	vec3 reflc = normalize(reflect(-lr, norm));
	vec3 eye = normalize(viewPos - FragPos);
	float spec = pow(max(dot(eye, reflc),0),material.shininess);
	vec3 specc = spec *light.specular *texture(map,TexCoords).rgb;
	colour = ambient + diff + specc;
	FragColour = vec4(colour,1.0f);
}