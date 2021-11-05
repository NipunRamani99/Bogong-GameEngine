#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
in VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 pos;
} vs_out[3];
out VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 pos;
} gs_out;

uniform float time;
vec3 GetNormal()
{
  vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
  return normalize(cross(a,b));
 }
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 100.0);
}
vec4 explode(vec4 position, vec3 normal, int index)
{
  float magnitude = 2.0;
  float rand_shit = rand(vec2(vs_out[index].pos.x, vs_out[index].pos.y));
  float rand_shit2 = abs(vs_out[index].pos.x) + abs(vs_out[index].pos.y);
  rand_shit2 *= 0.1f;
  vec3 direction = (smoothstep(10.0, 0.0f, time*(smoothstep(0.2f,0.35f,time)+rand_shit2))) * normal;//* ((sin(time)+1.0)/2.0) //* magnitude;
  return position + vec4(direction, 1.0);
}
void main() {
    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal, 0);
    gs_out = vs_out[0];
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal, 1);
    gs_out = vs_out[1];
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal, 2);
    gs_out = vs_out[2];
    EmitVertex();

    EndPrimitive();
}
