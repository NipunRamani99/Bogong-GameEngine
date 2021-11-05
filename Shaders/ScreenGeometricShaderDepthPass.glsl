#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

uniform mat4 shadowProjections[6];
uniform vec3 shadowLightDir[6];
out GS_OUT {
    mat4 shadowProjection;
    vec2 pos;
    vec3 shadowLightDir;
} gs_out;
in VS_OUT{
    vec2 pos;
} gs_in[];
void main() {
    for(int face = 0; face <6; ++face) {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i) {
            gl_Position = vec4(gs_in[i].pos.x, gs_in[i].pos.y, 0.0, 1.0);
            gs_out.shadowProjection = shadowProjections[face];
            gs_out.pos = gs_in[i].pos;
            gs_out.shadowLightDir = shadowLightDir[face];
            EmitVertex();
        }
        EndPrimitive();
    }
}