#version 120
const float SQRT_2 = 1.4142135623730951;
uniform float size, orientation, linewidth, antialias;
attribute vec3 position;
varying vec2 rotation;
varying float v_size;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	rotation = vec2(sin(orientation),cos(orientation));
	gl_Position = projection*view*model*vec4(position,1.0);
	v_size = SQRT_2 * size + 2.0*(linewidth + 1.5*antialias);
	gl_PointSize = v_size;
}