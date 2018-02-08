#version 410

// The modelview and projection matrices are no longer given in OpenGL 4.2
uniform mat4 M;
uniform mat4 MVP;
uniform mat4 N; // This is the inverse transpose of the MV matrix

// The vertex position attribute
layout (location = 0) in vec3 inVert;

// The texture coordinate attribute
layout (location = 1) in vec2 inUV;

// The vertex normal attribute
layout (location = 2) in vec3 inNormal;

out vec3 FragmentPosition;
out vec3 FragmentNormal;
out vec2 texCoord;

/************************************************************************************/
void main()
{
    // Set the position of the current vertex
    gl_Position = MVP * vec4(inVert, 1.0);
    FragmentPosition = vec3(M * vec4(inVert, 1.0));
    FragmentNormal = vec3(N * vec4(inNormal, 1.0));
    texCoord = inUV;
}

