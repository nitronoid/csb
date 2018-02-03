#version 410

// This is passed on from the vertex shader
in vec3 FragmentPosition;
in vec3 FragmentNormal;
in vec2 texCoord;


uniform bool addingColor;
uniform sampler2D ColourTexture;
uniform sampler2D NormalTexture;
uniform vec4 LightPosition;

uniform vec3 baseColor;
uniform vec3 dotsColor;

layout ( location = 0 ) out vec4 FragColor;


/* light copied from vert */
struct LightInfo
{
		vec4 Position; // Light position in eye coords.
		vec3 La; // Ambient light intensity
		vec3 Ld; // Diffuse light intensity
		vec3 Ls; // Specular light intensity
};

// We'll have a single light in the scene with some default values
uniform LightInfo Light = LightInfo
																	(
																		vec4( 10, 25.0, 30.0, 1.0 ),   // position
																		vec3( 2.5, 2.5, 2.5 ),        // La
																		vec3( 1.0, 1.0, 1.0 ),        // Ld
																		vec3( 1.0, 1.0, 1.0 )         // Ls
																	);


/* material copied from vert */
// The material properties of our object
struct MaterialInfo
{
		vec3 Ka; // Ambient reflectivity
		vec3 Kd; // Diffuse reflectivity
		vec3 Ks; // Specular reflectivity
		float Shininess; // Specular shininess factor
};

// The object has a material
uniform MaterialInfo Material = MaterialInfo
																						(
																						vec3(0.1, 0.1, 0.1),    // Ka
																						vec3(1.0, 1.0, 1.0),    // Kd
																						vec3(1, 1, 1),    // Ks
																						4.0                    // Shininess
																						);

/** From http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/ */
mat4 rotationMatrix( vec3 axis, float angle )
{
		//axis = normalize(axis);
		float s = sin( angle );
		float c = cos( angle );
		float oc = 1.0 - c;
		return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
		oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
		oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
		0.0,                                0.0,                                0.0,                                1.0 );
}

vec3 rotateVector( vec3 src, vec3 tgt, vec3 vec )
{
	float angle = acos( dot( src, tgt ) );

	if ( angle == 0 )
	{
		return vec;
	}
	vec3 axis = normalize( cross( src, tgt ) );
	mat4 R = rotationMatrix( axis, angle );
	vec4 norm = R * vec4( vec, 1.0f );
	return norm.xyz / norm.w;
}

/** https://www.shadertoy.com/view/XtV3z3 **/
float texture_lum(sampler2D _texture, vec2 _uv)
{
	vec3 rgb = texture( _texture, _uv ).rgb;
	return 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b;
}

/** https://www.shadertoy.com/view/XtV3z3 **/
vec3 toNormal()
{
	float r = 0.00078125f; // 1 / width of the texture

	float x0 = texture_lum( ColourTexture, vec2( texCoord.x + r, texCoord.y ) );
	float x1 = texture_lum( ColourTexture, vec2( texCoord.x - r, texCoord.y ) );
	float y0 = texture_lum( ColourTexture, vec2( texCoord.x, texCoord.y + r ) );
	float y1 = texture_lum( ColourTexture, vec2( texCoord.x, texCoord.y - r ) );

	vec3 n = normalize( vec3( x1 - x0, y1 - y0, 1.0f ) );

	return n * 0.5 + 0.5;
}

vec4 useColor(vec3 _LightIntensity)
{
	if ( texture( ColourTexture, texCoord ).r < 0.4f )
			return vec4( _LightIntensity *  dotsColor, 1.0 );
	else
			return vec4( _LightIntensity * baseColor , 1.0 );
}


vec3 PhongReflection(vec3 _s, vec3 _n, vec3 _v)
{
	vec3 r = reflect( -_s, _n );

	 // Compute the light from the ambient, diffuse and specular components
		return vec3(
							Light.La * Material.Ka +
							Light.Ld * Material.Kd * max( dot( _s, _n ), 0.0 ) +
							Light.Ls * Material.Ks * pow( max( dot( r, _v ), 0.0 ), Material.Shininess ) );
}

vec3 FresnelReflection(vec3 _s, vec3 _n, vec3 _v)
{

	float base = dot( _v, _s );
	float exponential = pow( base, 5.0 );
	float fresnel;
	vec3 r = reflect( -_s, _n );
	fresnel = exponential + .028f * ( 1.0 - exponential );

	return vec3(
				Light.La * Material.Ka +
				Light.Ld * Material.Kd * max( dot( _s, _n ), 0.0 ) +
				Light.Ls * Material.Ks * pow( max( dot( r, _v ), 0.0 ), Material.Shininess ) * fresnel );
}

void main()
{
	vec3 n = normalize( FragmentNormal ); // normal

	vec3 s = normalize( vec3( LightPosition.xyz ) - FragmentPosition ); // Calculate the light vector

	vec3 v = normalize( -vec3( FragmentPosition ) ); // eye

	vec3 tgt = normalize( toNormal() * 2.0 - 1.0 );

	vec3 src = vec3( 0.0, 0.0, 1.0 );
	n = rotateVector( src, tgt, n);

//	vec3 LightIntensity = FresnelReflection(s, n, v);
	vec3 LightIntensity = PhongReflection(s, n, v);

	if ( addingColor )
		FragColor = useColor(LightIntensity);
	else
		FragColor = vec4( texture( ColourTexture, texCoord ).rgb, 1);
}
