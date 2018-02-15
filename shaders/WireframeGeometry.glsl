#version 420 // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in VertexData
{
    vec3 vert;
    vec3 norm;
    vec2 uv;
} inData[];


out FragData
{
    vec3 vert;
    vec3 norm;
    vec2 uv;
    noperspective vec3 wireframeDist;
} outData;

out vec2 outUV;
out vec3 outVert;
out vec3 outNormal;


void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        outData.vert = inData[i].vert;
        outData.norm = inData[i].norm;
        outData.uv = inData[i].uv;
        outData.wireframeDist = vec3(0.0);
        outData.wireframeDist[i] = 1.0;
        EmitVertex();
    }
}
