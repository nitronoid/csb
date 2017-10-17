#version 410

in vec3 FragmentPosition;
in vec2 texCoord;

uniform sampler2D renderedTexure;
out vec4 FragColor;

void main()
{
	FragColor = vec4( texture( renderedTexure, texCoord ).rgb, 1.0f );
}
