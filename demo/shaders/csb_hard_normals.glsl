#version 420 // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable


layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in VertexData
{
  vec3 vert;
  vec2 uv;
} inData[];

out FragData
{
  vec3 vert;
  vec3 norm;
  vec2 uv;
} outData;

uniform mat4 MVP;
uniform vec3 camPos;

void main(void)
{
  vec3 norm = normalize(cross(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz,
      gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz));

  for (int i = 0; i < 3; ++i)
  {
    gl_Position = gl_in[i].gl_Position;
    outData.vert = inData[i].vert;
    outData.uv = inData[i].uv;
    outData.norm = norm;

    EmitVertex();
  }
  EndPrimitive();
}
