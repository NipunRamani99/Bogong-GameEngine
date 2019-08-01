#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec4 frag;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 fragColor;
const float SQRT_2 = 1.4142135623730951;
varying float v_size;
float linewidth = 1.0;
float antialias = 0.1;
float size = 4.0;
void main() {
	gl_Position = projection * view*model*vec4(vertPos, 1);
	v_size = SQRT_2 * size + 2.0*(linewidth + 1.5*antialias);
	fragColor = frag;
	gl_PointSize = v_size;
}
