#version 410

// The vertex position attribute
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TexCoord;

out vec3 FragmentPosition;
out vec2 texCoord;

/************************************************************************************/
void main()
{
	texCoord = TexCoord;
	FragmentPosition = VertexPosition;
	gl_Position = vec4(VertexPosition, 1.0);
}

