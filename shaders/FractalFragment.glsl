#version 430

// The output colour. At location 0 it will be sent to the screen.
layout (location=0) out vec4 fragColor;

// The uv coordinates of this fragment
in vec2 FragmentUV;

// The time elapsed (by default there is no animation)
uniform float t = 0.0;

// Define our color palette to make a nice colour ramp (more options are available at
// http://iquilezles.org/www/articles/palettes/palettes.htm
uniform vec3 ColorPalette[4] = vec3[](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.00, 0.10, 0.20));
//uniform vec3 ColorPalette[4] = vec3[](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.00, 0.33, 0.67));

// Cosine based palette from http://iquilezles.org/www/articles/palettes/palettes.htm
vec3 Palette(in float t) {
    return ColorPalette[0] + ColorPalette[1]*cos( 6.28318*(ColorPalette[2]*t+ColorPalette[3]) );
}



// ** Here you will have to implement some code to create Julia or Mandelbrot fractals. Appropriate this from the following link:
// ** http://nuclear.mutantstargoat.com/articles/sdr_fract/

void main() {
    // Determine the fractal position from the fragment UV coordinates (these normally within [0,1] - they need to be rescaled
    // to [-0.5,0.5] for the fractal formula)
    vec2 fracpos = FragmentUV - vec2(0.5);

    // Determine the colour from the
    fragColor = vec4(Palette(FragmentUV.x), 1.0);
}

