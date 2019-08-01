#version 330 core
const float PI = 3.141592653589792323846264;
const float SQRT = 1.4142135623730951;
in vec4 fragColor;
out vec4 color;
vec4 fg_color = vec4( 1.0,0.0,0.0,1.0 );
vec4 bg_color = vec4( 0.0,1.0,0.0,1.0 );
float linewidth = 1.0;
float antialias = 0.10;
float size = 1.0;
varying float v_size;
float disc(vec3 P, float size)
{
	float r1 = length(P) - size / 2.;
	float r2 = length(P) - size / 4.;
	return max(r1, -r2);
	
}
vec4 outline(float dist, float linewidth, float antialias, vec4 stroke, vec4 fill)
{
	float t = linewidth / 2.0 - antialias;
	float signed_distance = dist;
	float border_distance = abs(signed_distance) - t;
	float alpha = border_distance / antialias;
	alpha = 1.0;
	if (border_distance < 0.0)
		return stroke;
	else if (signed_distance < 0.0)
		return mix(fill, stroke, sqrt(alpha));
	else
		return vec4(stroke.rgb, stroke.a * alpha);
}
void main()
{
	vec3 P = gl_FragCoord.xyz - vec3(0.5, 0.5,0);
	float dist = disc(P*v_size, size);
	color = outline(dist, linewidth, antialias, fg_color, bg_color);
}