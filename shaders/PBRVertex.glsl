#version 420 // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable
// this demo is based on code from here https://learnopengl.com/#!PBR/Lighting
/// @brief the vertex passed in
layout (location = 0) in vec3 inVert;
/// @brief the normal passed in
layout (location = 2) in vec3 inNormal;
/// @brief the in uv
layout (location = 1) in vec2 inUV;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 MVP;
uniform mat4 N;
uniform mat4 M;

void main()
{
  gl_Position = MVP * vec4(inVert, 1.0);
  WorldPos = vec3(M * vec4(inVert, 1.0));
  Normal = vec3(N * vec4(inNormal, 1.0));
  TexCoords = inUV;
}
