uniform vec4 object_colour;
out vec4 FragColour;
in vec3 Normal;
in vec3 FragPos;
#ifdef HAS_UV
in vec2 texCoords;
uniform sampler2D s;
uniform samplerCube skybox;
#endif
uniform vec3 CamPos;

void main() {
#ifndef HAS_UV
	FragColour = object_colour;
#else
	FragColour = texture(s, texCoords);
	//Get ray to fragment
	vec3 ray  = normalize(FragPos - CamPos);
	//Reflect it using normal
	vec3 refl = reflect(ray, normalize(Normal));
	//See where it lands using sampler
	FragColour = vec4(texture(skybox, refl).rgb,1.0);
	//Mix the colors
	//FragColour = texture(s, texCoords);
#endif
}