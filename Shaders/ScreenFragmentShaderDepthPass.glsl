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
float sdPlane(vec3 p, vec4 n)
{
	// n must be normalized
	return dot(p, n.xyz) + n.w;
}
float GetDist(vec3 p, out int id)
{
	vec4 c = vec4(0.0, 1.90, 6.0, 1.0);
	float circ = length(p - c.xyz) - c.w;
	float plane = sdPlane(p,vec4(0,1.0f,0.0,0.0f));
	float plane2 = sdPlane(p, vec4(-1, 0, 0, 7));
	float plane3 = sdPlane(p, vec4(0, 0, -1, 15));
	float minval = plane;

	if (minval <= SURFACE_DIST)
	{
		if (minval == plane) id = 2;
	}
	return minval;
}
float RayMarch(vec3 ro, vec3 rd, out int id) {
	float d = 0.0;
	vec3 p = ro;
	for (int i = 0; i < MAX_STEPS; i++)
	{
		p = ro + rd * d;
		float dS = GetDist(p, id);
		d += dS;
		if (dS < SURFACE_DIST || d > MAX_DISTANCE) break;
	}
	return d;
}
vec3 GetNormal(vec3 p) {
	vec3 norm = vec3(0);
	vec2 e = vec2(0.01, 0.0);
	int id = 0;
	float d = GetDist(p, id);
	norm = vec3(d) - vec3(GetDist(p - e.xyy, id), GetDist(p - e.yxy, id), GetDist(p - e.yyx, id));
	return normalize(norm);
}
void calcCamera(out vec3 ro, out vec3 ta)
{
	float an = sin(0.1*iTime);
	ro = vec3(6.0*cos(an), 1., 6.0 + 5.0*sin(an));
	ta = vec3(0.0, 1.0, 6.0);
}
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
    //intersect(ddx_ro, ddx_rd, tx);
    //intersect(ddy_ro, ddy_rd, ty);
    vec3 p = ro + rd * t;
   // vec3 px = ddx_ro + ddx_rd * tx;
  //  vec3 py = ddy_ro + ddy_rd * ty;  
    gl_FragDepth = CalculateDepth(p);
 }