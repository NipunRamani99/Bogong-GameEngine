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
struct PointLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant ;
	float linear ;
	float quadratic ;
};
struct DirLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};
struct SpotLight {
	vec3 light_pos;
	vec3 light_dir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float cutoff;
};
uniform vec3 light_pos;
uniform SpotLight light;
uniform DirLight dirlight;
uniform sampler2D s;
uniform sampler2D map;
float outercutoff = cos(radians(15.0f));
void main() {
	//float dist = length(FragPos - light_pos);
    //float attenuation = 1.0f / (light.constant + light.linear *dist + light.quadratic * dist*dist);
	vec3 colour = vec3(1.0f);
	vec3 ambient= light.ambient*texture(s, TexCoords).rgb;
	vec3 norm   = normalize(Normal);
	vec3 lr     = normalize( light.light_pos-FragPos);
	float ang   = ( dot(lr, normalize(-light.light_dir)) );
	float intensity = (ang - light.cutoff) / (light.cutoff - outercutoff);
	intensity = clamp(intensity, 0.0, 1.0);
	float dif = max(dot(lr, norm), 0.0);
	vec3 diff = dif * light.diffuse*texture(s, TexCoords).rgb;
	vec3 reflc = normalize(reflect(-lr, norm));
	vec3 eye = normalize(viewPos - FragPos);
	float spec = pow(max(dot(eye, reflc), 0), material.shininess);
	vec3 specc = spec * light.specular*texture(map, TexCoords).rgb;
	colour = ambient + intensity*(specc+diff);
	FragColour  = vec4(colour, 1.0f);
}