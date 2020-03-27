#version 330 core
in vec3 Normal;
out vec4 FragColour;
uniform vec4 object_colour;
uniform vec3 light_pos;
uniform float ambient_str = 1.0f;
uniform float specular_str = 1.0f;
vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
in vec3 FragPos;
uniform vec3 viewPos;
void main() {
	FragColour     = vec4((object_colour.xyz)*ambient_str,1.0f);
	vec3 norm      = normalize(Normal);
	vec3 lightDir  = normalize(light_pos - FragPos);
	float diff     = max(dot(norm, lightDir), 0.0);
	vec3 diffuse   = diff * lightColour;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specular_str * spec*lightColour;
	vec3 result    = ( vec3(ambient_str) + diffuse+specular) * object_colour.xyz;
	FragColour     = vec4(result, 1.0);
}