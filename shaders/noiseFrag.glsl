#version 410

// noise from https://www.shadertoy.com/view/4djGRh

// This is passed on from the vertex shader
in vec3 FragmentPosition;
in vec2 texCoord;
//uniform sampler2D renderedTexture;
uniform float numOfCells;
uniform float numOfTiles;
out vec4 FragColor;

//#define TILES 2.0


vec2 Hash2( vec2 p )
{
	float r = 523.0 * sin( dot( p, vec2( 53.3158, 43.6143 ) ) );
	return vec2( fract( 15.32354 * r ), fract( 17.25865 * r ) );
}

//------------------------------------------------------------------------

float Cells(in vec2 p)
{
	p *= numOfCells;
  float d = 1.0e10;
	for ( int xo = -1; xo <= 1; xo++ )
  {
		for ( int yo = -1; yo <= 1; yo++ )
		{
			vec2 tp = floor( p ) + vec2( xo, yo );
			tp = p - tp - Hash2( mod( tp, numOfCells / numOfTiles ) );
			d = min( d, dot( tp, tp ) );
		}
  }
	return sqrt( d );
}

//------------------------------------------------------------------------

void main()
{
	 FragColor = vec4( Cells( texCoord ) );
}
