#version 420                                            // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable

layout (location=0) out vec4 fragColor;

// The uv coordinates of this fragment
in vec2 FragmentUV;

// The time elapsed (by default there is no animation)
uniform float t = 0.0;

// The number of iterations of this fractal
uniform int iter = 100;

// The signature for our fractal function
subroutine vec3 fractalFunctionType(vec2);

// This uniform variable indicates which fractal function to use
subroutine uniform fractalFunctionType fractalFunction;

// Define our color palette to make a nice colour ramp
uniform vec3 ColorPalette[4] = vec3[](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.00, 0.10, 0.20));
//uniform vec3 ColorPalette[4] = vec3[](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.00, 0.33, 0.67));

// Cosine based palette from http://iquilezles.org/www/articles/palettes/palettes.htm
vec3 Palette(in float t) {
    return ColorPalette[0] + ColorPalette[1]*cos( 6.28318*(ColorPalette[2]*t+ColorPalette[3]) );
}

// The inner loop of both fractal subroutines is shared (from http://nuclear.mutantstargoat.com/articles/sdr_fract/)
vec3 evalFractal(in vec2 z, in vec2 c) {
    int i;
    for(i=0; i<iter; i++) {
          float x = (z.x * z.x - z.y * z.y) + c.x;
          float y = (z.y * z.x + z.x * z.y) + c.y;
          if((x * x + y * y) > 4.0) break;
          z.x = x;
          z.y = y;
    }
    return Palette(((i == iter) ? 0.0 : float(i)) / iter);
}

// Calculate the Julia Fractal (from http://nuclear.mutantstargoat.com/articles/sdr_fract/)
subroutine (fractalFunctionType) vec3 JuliaFunction(vec2 pos) {
    vec2 c;
    // This evaluates the seed based on the elapsed time
    c.x = (sin(cos(t *0.1) * 10.0) + cos(t * 2.0)*0.25 + sin(t * 3.0) * 0.1667) * 0.8;
    c.y = (cos(sin(t *0.1) * 10.0) + sin(t * 2.0)*0.25 + cos(t * 3.0) * 0.1667) * 0.8;
    return evalFractal(vec2(3.0,2.0)*pos, c);
}

// Calculate Mandelbrot Fractal (from http://nuclear.mutantstargoat.com/articles/sdr_fract/)
subroutine (fractalFunctionType) vec3 MandelbrotFunction(vec2 pos) {
    float mixt = fract(t);
    vec2 center = vec2(mix(1.0, 1.31, mixt), 0.0);
    float scale = mix(0.5, 0.12, mixt);
    vec2 c = vec2(1.3333,1.0) * pos * scale - center;
    return evalFractal(c,c);
}

void main() {
    // Determine the fractal position from the fragment UV coordinates (these normally within [0,1])
    vec2 fracpos = FragmentUV - vec2(0.5);

    // Now execute the use specified blur function on this pixel based on the depth difference
    fragColor = vec4(fractalFunction(fracpos), 1.0);
    //fragColor = vec4(Palette(FragmentUV.x), 1.0);
}
