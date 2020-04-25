#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;
out vec4 FragColour;
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
uniform Material material;
struct PointLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 pos;
	float constant ;
	float linear;
	float quadratic ;
	
};
struct DirectionLight {
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
	float cutOff;
	float outerCutOff;
};

uniform vec3 light_pos;
uniform PointLight     point_light[10];
uniform SpotLight      spot_light[10];
uniform DirectionLight direction_light[10];
uniform int num_point_light     = 1;
uniform int num_spot_light      = 0;
uniform int num_direction_light = 0;

float outercutoff = cos(radians(15.0f));
#ifdef MATERIAL_WITH_TEX
vec4 GetPointLightResult() {
	vec4 result=vec4(0.0);
	for (int i = 0; i < num_point_light; i++) {
		vec3 pos   = point_light[i].pos;
		vec3 lr    = normalize(pos-FragCoord);
		vec3 norm  = normalize(Normal);
		vec3 eyer  = normalize(cam_pos - FragCoord);
		vec3 refl  = reflect(-lr,norm);

		float dif  = dot(lr, norm);
		float spec = pow(max(dot(refl, eyer),0.0), material.shininess);
		vec4 ambient = vec4(point_light[i].ambient,1.0) * texture(material.diffuse, UV).rgba;
		vec4 diffuse = dif * vec4(point_light[i].diffuse,1.0) * texture(material.diffuse,UV).rgba;
		vec4 specular = spec * vec4(point_light[i].specular, 1.0)*texture(material.specular, UV).rgba;
		result += ambient + diffuse + specular;
	}
	return result;
}
vec4 GetSpotLightResult() {
	vec4 result;
	for (int i = 0; i < num_spot_light; i++) {
		vec3 lightDir   = normalize(spot_light[i].position - FragCoord);
		vec3 normal     = normalize(Normal);
		vec3 viewDir    = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lightDir, normal);
		float dif       = max(dot(normal, lightDir), 0.0);
		float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		float distance  = length(spot_light[i].position - FragCoord);
		float attenuation = 1.0 / (spot_light[i].constant + spot_light[i].linear * distance + spot_light[i].quadratic * (distance * distance));
		float theta     = dot(lightDir, normalize(-spot_light[i].direction));
		float epsilon   = spot_light[i].cutOff - spot_light[i].outerCutOff;
		float intensity = clamp((theta - spot_light[i].outerCutOff) / epsilon, 0.0, 1.0);
		
		
		vec4 ambient    = vec4(light.ambient,1.0)  * texture(material.diffuse, TexCoords);
		vec4 diffuse    = vec4(light.diffuse,1.0)  * diff * texture(material.diffuse, TexCoords);
		vec4 specular   = vec4(light.specular,1.0) * spec * texture(material.specular, TexCoords);
		ambient        *= attenuation * intensity;
		diffuse        *= attenuation * intensity;
		specular       *= attenuation * intensity;
		result += ambient + diffuse + specular;
	}
	return result;
}
vec4 GetDirectionLightResult() {
	vec4 result;
	for (int i = 0; i < num_direction_light; i++) {
		vec3 lr = normalize(-direction_light[i].direction);
		vec3 normal = normalize(Normal);
		vec3 eyer = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lightDir, normal);
		float dif     = max(dot(normal, lr), 0.0);
		float spec    = pow(max(dot(eyer, reflectDir), 0.0), material.shininess);
		vec4 ambient  = texture(material.diffuse, UV).rgba * vec4(direction_light[i].ambient, 1.0);
		vec4 diffuse  = diff * texture(material.diffuse, UV).rgba  * vec4(direction_light[i].diffuse, 1.0);
		vec4 specular = spec * texture(material.specular, UV).rgba  * vec4(direction_light[i].specular, 1.0);
		result += specular + diffuse + ambient;
	}
	return result;
}

#else
vec4 GetPointLightResult() {
	vec4 result = vec4(0.0);
	for (int i = 0; i < num_point_light; i++) {
		vec3 pos = point_light[i].pos;
		vec3 lr = normalize(pos - FragCoord);
		vec3 norm = normalize(Normal);
		vec3 eyer = normalize(cam_pos - FragCoord);
		vec3 refl = reflect(-lr, norm);

		float dif     = dot(lr, norm);
		float spec    = pow(max(dot(refl, eyer), 0.0), material.shininess);
		vec4 ambient  = vec4(point_light[i].diffuse, 1.0) * vec4(material.ambient,1.0);
		vec4 diffuse  = dif  * vec4(point_light[i].diffuse, 1.0) * vec4(material.diffuse,1.0);
		vec4 specular = spec * vec4(point_light[i].specular, 1.0)* vec4(material.specular, 1.0);
		result += ambient + diffuse + specular;
	}
	return result;
}
vec4 GetSpotLightResult() {
	vec4 result;

	return result;
}
vec4 GetDirectionLightResult() {
	vec4 result;
	for (int i = 0; i < num_direction_light; i++) {
		vec3 lr       = normalize(-direction_light[i].direction);
		vec3 normal   = normalize(Normal);
		vec3 eyer     = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lightDir, normal);
		float dif     = max(dot(normal,lr),0.0);
		float spec    = pow(max(dot(eyer, reflectDir), 0.0), material.shininess);
		vec4 ambient  = vec4(material.ambient, 1.0) * vec4(direction_light[i].ambient, 1.0);
		vec4 diffuse  = diff * vec4(material.diffuse, 1.0) * vec4(direction_light[i].diffuse, 1.0);
		vec4 specular = spec * vec4(material.specular, 1.0) * vec4(direction_light[i].specular, 1.0);
		result += specular + diffuse + ambient;
	}
	return result;
}
#endif



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