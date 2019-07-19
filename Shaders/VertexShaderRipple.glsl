#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec4 frag;
layout(location = 2) in vec3 norm;
uniform vec3 lightLocation;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 fragColor;
float vLightIntensity;
out float lightIntensity;
void main(){
    vec3 Pos = vertPos;
	vec4 pos = projection*view*model*vec4(Pos,1);
	vec4 modelPos = model*vec4(Pos,1);
	vLightIntensity = dot(normalize(lightLocation-vec3(modelPos)),norm);
	vLightIntensity = 0.3 + max(vLightIntensity,0.0);
	lightIntensity = clamp(vLightIntensity,0,1);
	gl_Position = pos;
	fragColor = frag;
}