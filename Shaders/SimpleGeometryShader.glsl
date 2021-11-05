layout(points) in;
layout(triangle_strip, max_vertices = 5) out;
in VS_OUT{
    vec3 color;
} gs_in[];
out vec3 fColor;
void build_house(vec4 position)
{
    fColor = gs_in[0].color;
    //bottom right
    gl_Position = position + vec4(-0.1, -0.20, 0.0, 0.0);
    EmitVertex();

    //bottom left
    gl_Position = position + vec4(0.10, -0.2, 0.0, 0.0);
    EmitVertex();

    //top right
    gl_Position = position + vec4(-0.10, -0.1, 0.0, 0.0);
    EmitVertex();

    //top left
    gl_Position = position + vec4(0.10, -0.1, 0.0, 0.0);
    EmitVertex();

    //top
    gl_Position = position + vec4(0.0, 0.10, 0.0, 0.0);
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
void main() {
    build_house(gl_in[0].gl_Position);
}