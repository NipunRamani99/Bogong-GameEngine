out vec4 FragColor;

in vec3 TexCoords;
uniform float u_time;
uniform samplerCube skybox;

void main()
{
	//vec3 dir = vec3(abs(TexCoords.x), TexCoords.y, TexCoords.z);
	FragColor = texture(skybox, TexCoords);
	//vec3 color = vec3(0.0,0.0,1.0) * TexCoords.y;
	//FragColor = vec4(color, 1.0);
}