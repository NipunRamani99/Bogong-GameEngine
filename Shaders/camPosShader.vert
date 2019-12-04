#version 330 core
layout(location=0)in vec3 pos;
layout(location=1)in vec4 color;
uniform vec3 cameraPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec4 fragColor;
void main()
{
	vec4 vertical_pos = vec4(cameraPos,1.0);
	vertical_pos.y = 0.0;
	vertical_pos = projection*view*model*(vertical_pos);
	gl_Position = vertical_pos;
	fragColor = color;
}