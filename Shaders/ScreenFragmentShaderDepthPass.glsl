#version 330 core
#define MAX_STEPS 100
#define MAX_DISTANCE 100.0
#define SURFACE_DIST 0.01
#define THRESHOLD 0.01
in GS_OUT{
    mat4 shadowProjection;
    vec2 pos;
    vec3 shadowLightDir;
} gs_out;

uniform float iTime = 50.0f;

vec2 iResolution = vec2(1280, 640);

uniform vec3 cam_pos;
uniform float screen_width;
uniform float screen_height;
uniform float nearVal;
uniform float farVal;
vec2 pos = vec2(0.0);
const vec3 Csky = vec3(0.75, 0.75, 0.75);
uniform float thetaD = 45.0f;
void calcRayForPixel(in vec2 pix, out vec3 Ro, out vec3 Rd)
{
	vec2 p = pix;
	p /= iResolution.xy;
	vec3 u, v, w;
	float theta = radians(thetaD);
	float half_height =tan(theta / 2.0f);
	float half_width = screen_width/screen_height * half_height;
	vec3 origin = cam_pos;
	w = normalize(gs_out.shadowLightDir);
	u = normalize(cross(w,vec3(0.,1.,0.)));
	v = normalize(cross(u,w ));
 	vec3 lower_left_corner = cam_pos - half_width * u - half_height * v +w;
	vec3 horizontal        = 2.0f*half_width  *u;
	vec3 vertical          = 2.0f*half_height *v;
	Ro = cam_pos;
	Rd = normalize(lower_left_corner + p.x * horizontal + p.y * vertical-cam_pos);
	//Rd = normalize(p.x * u + p.y * v + 2.0*w);
}
vec2 texCoords(vec3 p, int id)
{
	vec2 m;
	if (id == 2)
	{
		m = p.xz;
	}
	return 2.0*m;
}
// triangular signal
vec2 tri(in vec2 x)
{
	vec2 h = fract(x*.5) - .5;
	return 1. - 2.*abs(h);
}
float checkersTexture(in vec2 p)
{
	vec2 s = sign(fract(p*0.05) - 0.05);
	return 0.05 - 0.05*s.x*s.y;
}
float checkersTextureGradBox(in vec2 p, in vec2 ddx, in vec2 ddy)
{
	// filter kernel
	vec2 w = max(abs(ddx), abs(ddy)) + 0.01;
	// analytical integral (box filter)
	vec2 i = 2.0*(abs(fract((p - 0.5*w) / 2.0) - 0.5) - abs(fract((p + 0.5*w) / 2.0) - 0.5)) / w;
	// xor pattern
	return 0.5 - 0.5*i.x*i.y;
}
float CalculateDepth(vec3 p) {
	vec4 clip_space = gs_out.shadowProjection*vec4(p,1.0f);
	float clip_space_depth = clip_space.z / clip_space.w;
    //float depth = (((farVal-nearVal)*(clip_space_depth))+nearVal+farVal)/2.0f;
    return clip_space_depth;
}
float iPlane(in vec3 ro, in vec3 rd) {
    return -ro.y / rd.y;
}
float intersect(in vec3 ro, in vec3 rd, out float resT) {
    resT = 1000.0;
    float t = -1.0;
    float tpla = iPlane(ro, rd);
    if (tpla > 0.0 && tpla < resT) {
        t = 1.0;
        resT = tpla;
    }
    return t;
}
void main() {
    pos = gs_out.pos;
    iResolution = vec2(screen_width, screen_height);
    vec2 newpos = vec2(0.0f,0.0f);
	newpos.x    = pos.x;
	newpos.y	= pos.y;
	vec3 ro, rd;
	vec3 ddx_ro, ddx_rd, ddy_ro, ddy_rd;
  	vec2 fragCoord = (newpos + vec2(1.0f,1.0f))*vec2(screen_width/2.0f, screen_height/2.0f);
    calcRayForPixel(fragCoord + vec2(0.0, 0.0), ro, rd);
    float t = 1.0;
    float tx = 1.0;
    float ty = 1.0;
    float id = intersect(ro, rd, t);
    vec3 p = ro + rd * t;
    gl_FragDepth = CalculateDepth(p);
 }