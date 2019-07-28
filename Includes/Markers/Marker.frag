#version 330 core
const float PI = 3.141592653589792323846264;
const float SQRT = 1.4142135623730951;
uniform float size, linewidth, antialias;
uniform vec4 fg_color, bg_color;
varying vec2 rotation;
varying float v_size;
float disc(vec2 P, float size)
{
	return	length(P) - size / 2;
}
vec4 outline(float dist, float linewidth, float antialias, vec4 stroke, vec4 fill)
{
	float t = linewidth / 2.0-antialias;
	float signed_distance = dist;
	float border_distance = abs(signed_distance) - t;
	float alpha = border_distance / antialias;
	alpha = exp(-alpha * alpha);
	if (border_distance < 0.0)
		return stroke;
	else if (signed_distance < 0.0)
		return mix(fill, stroke, sqrt(alpha));
	else
		return vec4(stroke.rgb, stroke.a * alpha);
}
void main()
{
	vec2 P = gl_PointCoord.xy - vec2(0.5, 0.5);
	P = vec2(rotation.x * P.x - rotation.y * P.y, rotation.x * P.x + rotation.y *P.y);
	float dist = disc(P*v_size, size);
	gl_FragColor = outline(dist, linewidth, antialias, fg_color, bg_color);
}
