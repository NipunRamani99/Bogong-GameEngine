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
#ifdef HAS_UV
in vec2 UV;
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
};
uniform Light light;
uniform Material material;
void main() {
	vec4 colour = vec4(1.0f,0.0f,1.0,1.0f);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_pos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);
	
#ifndef MATERIAL_WITH_TEX
	vec3 ambient    = material.ambient * light.ambient;
	vec3 diffuse    =  diff * material.diffuse *light.diffuse;
	vec3 specular   = specular_str*spec*light.specular*material.specular;
#else
	vec4 ambient, diffuse, specular;
	if (texture(material.diffuse, UV).a < 0.5) {
		discard;
	}
	else 
	{
		ambient = texture(material.diffuse, UV).rgba*vec4(light.ambient,1.0f);
		diffuse = diff * texture(material.diffuse, UV).rgba*vec4(light.diffuse,1.0f);
		specular = vec4(light.specular,1.0) * spec * texture(material.specular, UV).rgba;
	}
#endif
	vec4 result = ambient+diffuse+specular;
	FragColour  = result;
}