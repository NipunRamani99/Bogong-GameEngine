#version 330 core
layout (location = 0) in vec3 vertPos;
layout(location = 1) in vec4 frag;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 fragColor;
smooth out vec3 fragPos;
void main(){
	gl_Position = projection*view*model*vec4(vertPos,1);
	fragColor = frag;
	fragPos = vec3(model*vec4(vertPos,1.0));
}
