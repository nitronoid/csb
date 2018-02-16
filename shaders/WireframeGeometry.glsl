#version 420 // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

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
    for (int i = 0; i < 4; ++i)
    {
        int index = i % 3;
        gl_Position = gl_in[index].gl_Position;
        outData.vert = inData[index].vert;
        outData.norm = inData[index].norm;
        outData.uv = inData[index].uv;
        outData.wireframeDist = vec3(0.0);
        outData.wireframeDist[index] = 1.0;
        EmitVertex();
    }
    EndPrimitive();
}
