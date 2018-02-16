#version 420
#extension GL_EXT_gpu_shader4 : enable

/// @brief the in vertex
layout (location = 0) in vec3 inVert;
/// @brief the in uv
layout (location = 1) in vec2 inUV;
/// @brief the normal passed in
layout (location = 2) in vec3 inNormal;


// We need an MVP because the plane needs to be rotated
uniform mat4 MVP;
uniform mat4 N;
uniform mat4 M;

// Pass through the UV coordinates
out vec2 FragmentUV;

void main() {
    // Pass through the vertex UV's to be interpolated through rasterizations
    FragmentUV = inUV;

    // Set the position of the current vertex
    gl_Position = MVP * vec4(inVert, 1.0);
}

