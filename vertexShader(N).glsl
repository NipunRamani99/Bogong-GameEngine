#version 330 
layout (location = 0) in vec3 vertPos;
layout(location = 3) in vec3 normals;
layout(location = 1) in vec4 frag;
out vec4 fragColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 Normals;
out vec3 fragPos;
void main(){
	gl_Position = projection*view*model*vec4(vertPos,1);
	fragColor = frag;
	Normals = normals;
	fragPos = vec3(model*vec4(vertPos,1.0));

}
