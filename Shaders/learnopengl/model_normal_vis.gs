#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 Pos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out[3];

out VS_OUT {
    vec3 Pos;
    vec3 Normal;
    vec2 TexCoord;
} gs_out;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
vec3 getTriangleCenter(vec3 a, vec3 b, vec3 c){
    return (a+b+c)/3.0;
}

vec3 getSurfaceNormal(vec3 a, vec3 b, vec3 c){
    vec3 u = b - a;
    vec3 v = c - a;
    vec3 normal = vec3(0.0);
    normal.x = (u.y * v.z) - ( u.z * v.y );
    normal.y = (u.z * v.x) - ( u.x * v.z );
    normal.z = (u.x * v.y) - ( u.y * v.x );
    return normal;
}

void main() {
   vec3 a = vs_out[0].Pos.xyz;
   vec3 b = vs_out[1].Pos.xyz;
   vec3 c = vs_out[2].Pos.xyz;
   vec3 pos = getTriangleCenter(a, b, c);
   vec3 normal = normalize(getSurfaceNormal(a, b, c));
   
   vec3 pos1 = pos+0.01*normal;
   vec3 pos2 = pos-0.01*normal;

   gl_Position = projection*view*model*vec4(pos1,1.0);
   gs_out = vs_out[0];
   EmitVertex();

   gl_Position = projection*view*model*vec4(pos2,1.0);
   gs_out = vs_out[1];
   EmitVertex();

   EndPrimitive();
}