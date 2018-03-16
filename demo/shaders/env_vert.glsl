#version 420

// The vertex position attribute
layout (location=0) in vec3 inVert;

// The texture coordinate attribute
layout (location=1) in vec2 inUV;

// The vertex normal attribute
layout (location=2) in vec3 inNormal;

// These attributes are passed onto the shader (should they all be smoothed?)
smooth out vec3 FragmentPosition;
smooth out vec3 FragmentNormal;
smooth out vec2 FragmentTexCoord;

uniform mat4 M;            // model view matrix calculated in the App
uniform mat4 MVP;           // model view projection calculated in the app
uniform mat4 N;             // normal matrix calculated in the app

void main()
{
    // Transform the vertex normal by the inverse transpose modelview matrix
    FragmentNormal = normalize(vec3(N * vec4(inNormal, 1.0)));

    // Compute the unprojected vertex position
    FragmentPosition = vec3(M * vec4(inVert, 1.0) );

    // Copy across the texture coordinates
    FragmentTexCoord = inUV;

    // Compute the position of the vertex
    gl_Position = MVP * vec4(inVert,1.0);
}





