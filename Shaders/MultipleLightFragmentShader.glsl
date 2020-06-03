
in vec2 TexCoords;
in vec3 FragCoord;
in vec3 Normal;
uniform vec3 cam_pos;
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
	float constant;
	float linear;
	float quadratic;
	
};
struct DirectionLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

uniform vec3 light_pos;
uniform PointLight     point_light[10];
uniform SpotLight      spot_light[10];
uniform DirectionLight direction_light[10];
uniform int num_point_light     = 0;
uniform int num_spot_light      = 0;
uniform int num_direction_light = 0;

#ifdef MATERIAL_WITH_TEX
vec4 GetPointLightResult() {
	vec4 result=vec4(0.0);
	for (int i = 0; i < num_point_light; i++) {
		vec3 pos   = point_light[i].pos;
		vec3 lr    = normalize(pos-FragCoord);
		vec3 norm  = normalize(Normal);
		vec3 eyer  = normalize(cam_pos - FragCoord);
		vec3 refl  = reflect(-lr,norm);

		float dif  = max(dot(lr, norm),0.0);
		float spec = pow(max(dot(refl, eyer),0.0),64.0f);

		float distance = length(pos - FragCoord);
		float attenuation = 1.0 / (point_light[i].constant + point_light[i].linear * distance + point_light[i].quadratic * (distance * distance));
		
		vec4 ambient = vec4(point_light[i].ambient,1.0) * texture(material.diffuse, UV).rgba;
		vec4 diffuse = dif * vec4(point_light[i].diffuse,1.0) * texture(material.diffuse,UV).rgba;
		vec4 specular = vec4(spec * point_light[i].specular*texture(material.specular, UV).rgb,1.0f);
		ambient *= attenuation ;
		diffuse *= attenuation ;
		specular *= attenuation ;
		result += ambient + diffuse + specular;//+ diffuse + specular;
	}
	return result;
}
vec4 GetSpotLightResult() {
	vec4 result = vec4(0.0);
	for (int i = 0; i < num_spot_light; i++) {
		vec3 lightDir   = normalize(spot_light[i].position - FragCoord);
		vec3 normal     = normalize(Normal);
		vec3 viewDir    = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lightDir, normal);
		float diff       = max(dot(normal, lightDir), 0.0);
		float spec = 0.;
		if (material.shininess != 0.0f)
			spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			
		float distance  = length(spot_light[i].position - FragCoord);
		float attenuation = 1.0 / (spot_light[i].constant + spot_light[i].linear * distance + spot_light[i].quadratic * (distance * distance));
		float theta     = dot(lightDir, normalize(-spot_light[i].direction));
		float epsilon   = spot_light[i].cutOff - spot_light[i].outerCutOff;
		float intensity = clamp((theta - spot_light[i].outerCutOff) / epsilon, 0.0, 1.0);
		
		
		vec4 ambient    = vec4(spot_light[i].ambient,1.0)  * texture(material.diffuse, UV);
		vec4 diffuse    = vec4(spot_light[i].diffuse,1.0)  * diff * texture(material.diffuse, UV);
		vec4 specular   = vec4(spot_light[i].specular,1.0) * spec * texture(material.specular, UV);
		ambient *= attenuation;
		diffuse *= intensity*attenuation;
		specular *= intensity*attenuation;
		result += ambient + diffuse + specular;
	}
	return result;
}
vec4 GetDirectionLightResult() {
	vec4 result = vec4(0.0); 
	for (int i = 0; i < num_direction_light; i++) {
		vec3 lr = normalize(-direction_light[i].direction);
		vec3 normal = normalize(Normal);
		vec3 eyer = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lr, normal);
		float diff     = max(dot(normal, lr), 0.0);
		float spec = 0.0f;
		if(material.shininess != 0.0f)
			spec = pow(max(dot(eyer, reflectDir), 0.0), material.shininess);
		
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
	vec4 result = vec4(0.0);
	for (int i = 0; i < num_spot_light; i++) {
		vec3 lightDir = normalize(spot_light[i].position - FragCoord);
		vec3 normal = normalize(Normal);
		vec3 viewDir = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lightDir, normal);
		float diff = max(dot(normal, lightDir), 0.0);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		float distance = length(spot_light[i].position - FragCoord);
		float attenuation = 1.0 / (spot_light[i].constant + spot_light[i].linear * distance + spot_light[i].quadratic * (distance * distance));
		float theta = dot(lightDir, normalize(-spot_light[i].direction));
		float epsilon = spot_light[i].cutOff - spot_light[i].outerCutOff;
		float intensity = clamp((theta - spot_light[i].outerCutOff) / epsilon, 0.0, 1.0);


		vec4 ambient = vec4(spot_light[i].ambient, 1.0)   * vec4(material.ambient,1.0);
		vec4 diffuse = vec4(spot_light[i].diffuse, 1.0)   * diff * vec4(material.diffuse,1.0);
		vec4 specular = vec4(spot_light[i].specular, 1.0) * spec * vec4(material.specular,1.0) ;
		ambient *= attenuation * intensity;
		diffuse *= attenuation * intensity;
		specular *= attenuation * intensity;
		result += ambient + diffuse + specular;
	}
	return result;
}
vec4 GetDirectionLightResult() {
	vec4 result = vec4(0.0);
	for (int i = 0; i < num_direction_light; i++) {
		vec3 lr = normalize(-direction_light[i].direction);
		vec3 normal   = normalize(Normal);
		vec3 eyer     = normalize(cam_pos - FragCoord);
		vec3 reflectDir = reflect(-lr, normal);
		float diff     = max(dot(normal,lr),0.0);
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
	vec4 colour = GetPointLightResult() + GetSpotLightResult() + GetDirectionLightResult();
	colour.a = 1.0;
	FragColour = colour;
}