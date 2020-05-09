#define MAX_STEPS 100
#define MAX_DISTANCE 35.0
#define SURFACE_DIST 0.01
#define THRESHOLD 0.01
in vec2 pos;
out vec4 FragColour;
uniform float iTime = 50.0f;
vec2 iResolution = vec2(1280, 640);
uniform vec3 cam_dir;
uniform vec3 cam_pos;
uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;
//const vec3 Csky = vec3(0.902, 0.902, 0.980);
const vec3 Csky = vec3(0.75, 0.75, 0.75);
uniform float nearVal = 1.0f;
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
	//pix -> (0,0),(1280,640)
	//vec2 p = (2.0f*pix - iResolution.xy) / iResolution.y;
	vec2 p = pix;
	p /= iResolution.xy;
	//p -> ((-640,-320), (640,320))/iResolution.y
	vec3 u, v, w;
	float theta = radians(thetaD);
	float half_height =tan(theta / 2.0f);
	float half_width = 1280.0f/640.0f * half_height;
	vec3 origin = cam_pos;
	w = normalize(cam_dir);
	u = normalize(cross(w,vec3(0.,1.,0.)));
	v = normalize(cross(u,w ));
 	vec3 lower_left_corner = cam_pos - half_width * u - half_height * v +w;
	vec3 horizontal        = 2.0f*half_width  *u;
	vec3 vertical          = 2.0f*half_height *v;
	Ro = cam_pos;
	Rd = normalize(lower_left_corner + p.x * horizontal + p.y * vertical-cam_pos);
	//Rd = normalize(p.x * u + p.y * v + 2.0*w);
}
float Light(vec3 p)
{
	vec3 lightSource = vec3(2.*sin(iTime), 5., 6. + 2.*cos(iTime));
	vec3 l = normalize(lightSource - p);
	vec3 n = GetNormal(p);
	float diff = max(dot(n, l), 0.0);
	int id = 0;
	float d = RayMarch(p + n * SURFACE_DIST*2., l, id);
	if (d < length(p - lightSource)) diff = 0.01;
	return diff;
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
	vec4 clip_space = proj*view*model*vec4(p,1.0f);
	float clip_space_depth = clip_space.z / clip_space.w;
	float far = gl_DepthRange.far;
	float near = gl_DepthRange.near;
	float depth = (((far-near)*(clip_space_depth))+near+far)/2.0f;
	return depth;

}
void main() {
	vec2 newpos= vec2(0.0f,0.0f);
	newpos.x    = pos.x;
	newpos.y	= pos.y;
	vec3 ro, rd;
	vec3 ddx_ro, ddx_rd, ddy_ro, ddy_rd;
	vec2 fragCoord = (newpos + vec2(1.0f,1.0f))*vec2(640.0f, 320.0f);

	calcRayForPixel(fragCoord + vec2(0.0, 0.0), ro, rd);
	calcRayForPixel(fragCoord + vec2(1.0, 0.0), ddx_ro, ddx_rd);
	calcRayForPixel(fragCoord + vec2(0.0, 1.0), ddy_ro, ddy_rd);

	int id = 0;
	float d = RayMarch(ro, rd, id);
	float ddx = RayMarch(ddx_ro, ddx_rd, id);
	float ddy = RayMarch(ddy_ro, ddy_rd, id);

	vec3 p = ro + rd * d;
	vec3 px = ddx_ro + ddx_rd * ddx;
	vec3 py = ddy_ro + ddy_rd * ddy;
	float dif = Light(p);

	
	vec3 color = vec3(0.0f);
	if (id == 2) {
		vec2 uv = texCoords(p, id);
		vec2 uvx = texCoords(px, id) - uv;
		vec2 uvy = texCoords(py, id) - uv;
		color = vec3(1.0)*checkersTextureGradBox(uv, uvx, uvy);
		// color = vec3(1.0)*checkersTexture(uv); // unfiltered pattern.
		dif += 0.06;
		//color *= dif;
	}
	else {
		color = vec3(0.53f, 0.81f, 0.920f);
	}
	gl_FragDepth = CalculateDepth(p);
	float fog = 1.0f - exp(-0.1*d);
	color.rgb = mix(color.rgb, Csky, fog);
	FragColour = vec4(color, 1.0);
}